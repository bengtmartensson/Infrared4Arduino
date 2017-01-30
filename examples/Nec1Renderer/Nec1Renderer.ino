// Send nec1 122/27 (volume down for Yamaha) followed by 10 repeats

#include <Nec1Renderer.h>
#include <IrSenderPwm.h>

const IrSignal *irSignal;

void setup() {
    Serial.begin(115200);
    irSignal = Nec1Renderer::newIrSignal(122, 27); // volume_down for Yahama receivers
}

void loop() {
    irSignal->dump(Serial, true);
    IrSenderPwm::getInstance(true)->sendIrSignal(*irSignal, 11);
    delay(10000);
}
