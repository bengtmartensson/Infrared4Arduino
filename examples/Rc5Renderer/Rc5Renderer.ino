// Sends Rc5 0/1 twice with different toggle values.
// Turns e.g. a Philips TV to channel 11.

#include <Rc5Renderer.h>
#include <IrSenderPwm.h>

IrRenderer *renderer;

void setup() {
    renderer = new Rc5Renderer(0, 1);
}

void loop() {
    renderer->send(*IrSenderPwm::getInstance(true));
    delay(1000);
    renderer->send(*IrSenderPwm::getInstance(true));
    delay(10000);
}
