#include <IrReceiverSampler.h>
#include <HashDecoder.h>

#define RECEIVE_PIN 5U
#define BUFFERSIZE 200U
#define BAUD 115200

//#define DEBUG

// Just 4 random names
static const char* names[] = {
    "Play",
    "Stop",
    "Pause",
    "Rewind"
};

class Command{
public :
    const char* name;
    uint32_t code;

    Command() : name(nullptr), code(0UL) {};
    Command(const char* n, uint32_t c) : name(n),code(c) {}
};

unsigned numberCommands = sizeof (names) / sizeof (const char*);

static Command *learnedCommands;

IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    while (!Serial)
        ;

    Serial.print(F("Make sure you have a demodulating receiver at pin "));
    Serial.println(RECEIVE_PIN);

    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
    receiver->setEndingTimeout(20);

    learnedCommands = new Command[numberCommands];

    for (unsigned i = 0; i < numberCommands; i++) {
        const char* name = names[i];
        Serial.print(F("Now send the command we will call "));
        Serial.print(name);
        Serial.print(F("..."));

        do
            receiver->receive();
        while (receiver->isEmpty());
        Serial.println(F("OK"));

        uint32_t hash = HashDecoder::decodeHash(*receiver);
#ifdef DEBUG
        Serial.println(hash);
#endif
        Command cmd(name, hash);
        learnedCommands[i] = cmd;
        delay(250);
    }

#ifdef DEBUG
    Serial.println(F("Learned finished! Got the following table:"));
    for (unsigned i = 0; i < numberCommands; i++) {
        Serial.print(i);
        Serial.print('\t');
        Command command = learnedCommands[i];
        Serial.print(command.name);
        Serial.print('\t');
        Serial.println(command.code);
    }
#endif
    Serial.println(F("Now press these buttons, and occasionally other ones."));
    Serial.println();
}

void loop() {
    do
            receiver->receive();
    while (receiver->isEmpty());

    uint32_t hash = HashDecoder::decodeHash(*receiver);
#ifdef DEBUG
    Serial.println(hash);
#endif
    if (hash == 84696351) // NEC1 repeat, discard
        return;

    for (unsigned i = 0; i < numberCommands; i++) {
        Command cmd = learnedCommands[i];
        if (cmd.code == hash) {
            Serial.print(F("Thank you for sending the "));
            Serial.print(cmd.name);
            Serial.println(F(" command."));
            return;
        }
    }
    Serial.println(F("Unknown command, please try again."));
}
