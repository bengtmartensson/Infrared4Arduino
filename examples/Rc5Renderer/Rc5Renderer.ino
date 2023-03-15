// Sends Rc5 0/1 twice with different toggle values.
// Turns e.g. a Philips TV to channel 11.

#include <Rc5Renderer.h>
#include <IrSenderPwm.h>

IrSender *sender;
const IrSignal *signal_0;
const IrSignal *signal_1;

void setup() {
    sender = IrSenderPwm::getInstance(true);
    signal_0 = Rc5Renderer::newIrSignal(0, 1); // toggle = 0
    signal_1 = Rc5Renderer::newIrSignal(0, 1); // toggle = 1
}

void loop() {
    sender->sendIrSignal(*signal_0);
    delay(1000);
    sender->sendIrSignal(*signal_1);
    delay(10000);
}
