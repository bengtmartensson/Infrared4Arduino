#include <Arduino.h>
#include <Nec1Renderer.h>
#include <IrSenderPwm.h>

IrRenderer *renderer;

void setup() {
    renderer = new Nec1Renderer(122, 29); // power_on for Yahama receivers
}

void loop() {
    renderer->send(*IrSenderPwm::getInstance(true));
    delay(10000);
}
