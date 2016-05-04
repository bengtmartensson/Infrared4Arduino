// This sketch demonstrates the IrWidgetAggregating.
// It requires a non-demodulating sensor connected to the capture pin.

#include <Arduino.h>
#include <IrWidgetAggregating.h>

#define BUFFERSIZE 200U
#define BAUD 115200

IrWidgetAggregating *capturer;

void setup() {
    Serial.begin(BAUD);
    capturer = IrWidgetAggregating::newIrWidgetAggregating(BUFFERSIZE);
}

void loop() {
    capturer->capture();
    if (capturer->isEmpty())
        Serial.println(F("timeout"));
    else
        capturer->dump(Serial);
}
