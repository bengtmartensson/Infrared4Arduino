// This sketch uses the IrReceiveSampler to receive a signal, and tries to
// decode it as a NEC signal

#include <IrReceiverSampler.h>
#include <Nec1Decoder.h>

#define RECEIVE_PIN 5U
#define BUFFERSIZE 200U
#define BAUD 115200UL

static IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    while (!Serial)
        ;
    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
}

void loop() {
    receiver->receive();

    if (receiver->isEmpty())
        Serial.println(F("timeout"));
    else {
        Nec1Decoder decoder(*receiver);
        if (decoder)
            decoder.printDecode(Serial);
        else
            Serial.println(F("No decode as NEC"));
    }
}
