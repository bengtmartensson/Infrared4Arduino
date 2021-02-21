// This sketch demonstrates the IrWidgetAggregating.
// It requires a non-demodulating sensor connected to the capture pin.

#include <Arduino.h>
#include <IrWidgetAggregating.h>

static constexpr size_t BUFFERSIZE = 200U;
static constexpr uint32_t BAUD = 115200U;

IrWidgetAggregating *capturer;

void setup() {
    Serial.begin(BAUD);
    capturer = IrWidgetAggregating::newIrWidgetAggregating(BUFFERSIZE);
}

void loop() {
    capturer->capture();
    if (capturer->isEmpty())
        Serial.println(F("timeout"));
    else {
        capturer->dump(Serial);
        Serial.println();
    }
}
