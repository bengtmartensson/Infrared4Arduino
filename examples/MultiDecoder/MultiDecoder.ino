// This sketch demonstrates the MultiDecoder.
// It requires a demodulating sensor connected to pin RECEIVE_PIN.

#include <IrReceiverSampler.h>
#include <MultiDecoder.h>

#define RECEIVE_PIN 5U
#define BUFFERSIZE 200U
#define BAUD 115200UL

IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
}

void loop() {
    receiver->receive();

    if (receiver->isEmpty())
        Serial.println(F("timeout"));
    else {
        MultiDecoder decoder(*receiver);
        if (decoder)
            decoder.printDecode(Serial);
        else
            Serial.println(F("No decode"));
    }
}
