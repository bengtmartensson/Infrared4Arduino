// Sends Rc5 0/1 twice with different toggle values.
// Turns e.g. a Philips TV to channel 11.

#include <Rc5Renderer.h>
#include <IrSenderPwm.h>

IrSender *sender;

void setup() {
    sender = IrSenderPwm::getInstance(true);
}

void loop() {
    const IrSignal *signal = Rc5Renderer::newIrSignal(0, 1);
    sender->sendIrSignal(*signal);
    delay(1000);
    signal = Rc5Renderer::newIrSignal(0, 1);
    sender->sendIrSignal(*signal);
    delay(10000);
}
