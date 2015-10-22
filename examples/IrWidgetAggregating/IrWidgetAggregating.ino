#include <Arduino.h>
#include <IrWidgetAggregating.h>

IrWidgetAggregating *capturer;

void setup() {
    Serial.begin(9600);
    capturer = IrWidgetAggregating::newIrWidgetAggregating(200, Serial);
}

void loop() {
    capturer->reset();
    capturer->capture();
    if (capturer->isReady())
        capturer->dump(Serial);
    else
        Serial.println("timeout");
}
