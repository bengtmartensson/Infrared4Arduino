#include <IrReceiverSampler.h>
#include <MultiDecoder.h>

IrReceiver *receiver;

void setup() {
    Serial.begin(115200);
    receiver = IrReceiverSampler::newIrReceiverSampler(200, 5);
}

void loop() {
    receiver->receive();

    if (receiver->isEmpty())
        Serial.println("timeout");
    else {
        MultiDecoder decoder(*receiver);
        if (decoder.isValid())
            decoder.printDecode(Serial);
        else
            Serial.println("No decode");
    }
}
