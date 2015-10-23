#include <Arduino.h>
#include <Rc5Renderer.h>
#include <IrSenderPwm.h>

IrRenderer *renderer;

void setup() {
    renderer = new Rc5Renderer(0, 0);
}

void loop() {
    renderer->send(*IrSenderPwm::getInstance(true));
    delay(10000);
}
