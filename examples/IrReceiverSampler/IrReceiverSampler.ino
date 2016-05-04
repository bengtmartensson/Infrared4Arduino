// This sketch demonstrates the IrReceiverSampler.
// It requires a demodulating sensor connected to pin RECEIVE_PIN.

#include <Arduino.h>
#include <IrReceiverSampler.h>

#define RECEIVE_PIN 5U
#define BUFFERSIZE 200U
#define BAUD 115200

IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
}

void loop() {
    receiver->receive(); // combines enable, loop, disable

    if (receiver->isEmpty())
        Serial.println(F("timeout"));
    else
        receiver->dump(Serial);
}
