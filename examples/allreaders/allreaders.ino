#include <Arduino.h>
#include <IrWidgetAggregating.h>
#include <IrReceiverPoll.h>
#include <IrReceiverSampler.h>

IrReader *reader;
const unsigned int captureLength = 200;
const pin_t receiverPin = 5;
const microseconds_t markExcess = 50;

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(10000);
    Serial.flush();
    Serial.println(F("Enter s for sampler, p for poll, w for widget"));
    String line = Serial.readStringUntil('\r');
    char ch = line.charAt(0);
    Serial.println(F("Enter beginning timeout in milliseconds (default 5000)"));
    long beginningTimeout = Serial.parseInt();
    if (beginningTimeout == 0)
        beginningTimeout = 5000;
    Serial.println(F("Enter ending timeout in milliseconds (default 30)"));
    long endingTimeout = Serial.parseInt();
    if (endingTimeout == 0)
        endingTimeout = 30;

    reader = ch == 's'
            ? (IrReader*) IrReceiverSampler::newIrReceiverSampler(captureLength, receiverPin,
            false, markExcess, beginningTimeout, endingTimeout)
            : ch == 'p'
            ? (IrReader*) new IrReceiverPoll(captureLength, receiverPin,
            false, markExcess, beginningTimeout, endingTimeout)
            : (IrReader*) IrWidgetAggregating::newIrWidgetAggregating(captureLength,
            false, markExcess, beginningTimeout, endingTimeout);
    Serial.println(F("Now fire IR signals at your sensor."));
}

void loop() {
    reader->receive();
    if (reader->isEmpty())
        Serial.println(F("timeout"));
    else
        reader->dump(Serial);
}
