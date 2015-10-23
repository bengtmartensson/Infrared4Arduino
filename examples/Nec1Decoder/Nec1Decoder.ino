#include <IrReceiverSampler.h>
#include <Nec1Decoder.h>

IrReceiver *receiver;

void setup() {
    Serial.begin(9600);
    receiver = IrReceiverSampler::newIrReceiverSampler(200, 5);
}

void loop() {
    receiver->receive();

    if (receiver->isEmpty())
        Serial.println("timeout");
    else {
        IrDecoder *decoder = new Nec1Decoder(*receiver);
        if (decoder->isValid())
            decoder->printDecode(Serial);
        else
            Serial.println("No decode");
    }
}
