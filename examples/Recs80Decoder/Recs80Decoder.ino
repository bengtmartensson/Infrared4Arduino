// This sketch uses the IrReceiveSampler to receive a signal, and tries to
// decode it as a RECS80 signal

#include <IrReceiverSampler.h>
#include <Recs80Decoder.h>

#define RECEIVE_PIN 5U
#define BUFFERSIZE 200U
#define BAUD 115200
#define ENDING_TIMEOUT 35U

IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
    receiver->setEndingTimeout(ENDING_TIMEOUT);
}

void loop() {
    receiver->receive();

    if (receiver->isEmpty())
        Serial.println(F("timeout"));
    else {
        receiver->dump(Serial);
        Recs80Decoder decoder(*receiver);
        if (decoder.isValid())
            decoder.printDecode(Serial);
        else
            Serial.println(F("No decode"));
    }
}
