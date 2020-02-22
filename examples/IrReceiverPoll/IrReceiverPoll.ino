// This sketch demonstrates the IrReceiverPoll.
// It requires a demodulating sensor connected to pin RECEIVE_PIN.

#include <Arduino.h>
#include <IrReceiverPoll.h>

#define RECEIVE_PIN 5U
#define BUFFERSIZE 200U
#define BAUD 115200

IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    while (!Serial)
        ;
    receiver = new IrReceiverPoll(BUFFERSIZE, RECEIVE_PIN);
    Serial.println(F("Listening on pin "));
    Serial.println(receiver->getPin(), DEC);
}

void loop() {
    receiver->receive(); // combines enable, loop, disable

    if (receiver->isEmpty())
        Serial.println(F("timeout"));
    else
        receiver->dump(Serial);
}
