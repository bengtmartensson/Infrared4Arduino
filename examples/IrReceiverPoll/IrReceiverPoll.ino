// This sketch demonstrates the IrReceiverPoll.
// It requires a demodulating sensor connected to pin RECEIVE_PIN.

// This sketch runs on *anything* (at least with sufficient speed and memory)
// without any requirements on timers or interrupts.
// However ESP8266 seen to make problems...

#include <Arduino.h>
#include <IrReceiverPoll.h>

#ifdef ESP32
#define RECEIVE_PIN 4U
#elif ESP8266
#define RECEIVE_PIN 2U
#else
#define RECEIVE_PIN 5U
#endif

#define BUFFERSIZE 200U
#define BAUD 115200

IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    while (!Serial)
        ;
    receiver = new IrReceiverPoll(BUFFERSIZE, RECEIVE_PIN);
    Serial.print(F("Listening on pin "));
    Serial.println(receiver->getPin(), DEC);
}

void loop() {
    receiver->receive(); // combines enable, loop, disable

    if (receiver->isEmpty())
        Serial.println(F("timeout"));
    else
        receiver->dump(Serial);
}
