#include <Arduino.h>
#include <Nec1Renderer.h>
#include <IrSenderPwm.h>

IrRenderer *renderer;

void setup() {
    Serial.begin(9600);
    renderer = new Nec1Renderer(122, 29); // power_on for Yahama receivers
}

void loop() {
    ((const IrSignal&)renderer).dump(Serial);
    renderer->send(*IrSenderPwm::getInstance(true));
    delay(10000);
}
