#include <Arduino.h>
#include <IrWidgetAggregating.h>

IrWidgetAggregating *capturer;

void setup() {
    Serial.begin(115200);
    capturer = IrWidgetAggregating::newIrWidgetAggregating(200);
}

void loop() {
    capturer->reset();
    capturer->receive();
    if (capturer->isReady())
        capturer->dump(Serial);
    else
        Serial.println("timeout");
}
