// This sketch uses the IrReceiveSampler to receive a signal, and tries to
// decode it as a NEC signal

#include <IrReceiverSampler.h>
#include <Nec1Decoder.h>

static constexpr pin_t RECEIVE_PIN = 5U;
static constexpr size_t BUFFERSIZE = 200U;
static constexpr uint32_t BAUD = 115200UL;

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
