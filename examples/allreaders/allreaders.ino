#include <Arduino.h>
#include <IrWidgetAggregating.h>
#include <IrReceiverPoll.h>
#include <IrReceiverSampler.h>

static IrReader *reader;
constexpr unsigned int captureLength = 200U;
constexpr pin_t receiverPin = 5U;
constexpr microseconds_t markExcess = 50U;

void setup() {
    Serial.begin(115200UL);
    Serial.setTimeout(10000U);
    Serial.flush();
    Serial.println(F("Enter s for sampler, p for poll, w for widget"));
    String line = Serial.readStringUntil('\r');
    char ch = line.charAt(0);
    Serial.println(F("Enter beginning timeout in milliseconds (default 5000)"));
    long beginningTimeout = Serial.parseInt();
    if (beginningTimeout == 0U)
        beginningTimeout = 5000U;
    Serial.println(F("Enter ending timeout in milliseconds (default 30)"));
    long endingTimeout = Serial.parseInt();
    if (endingTimeout == 0U)
        endingTimeout = 30U;

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
    else {
        reader->dump(Serial);
        Serial.println();
    }
}
