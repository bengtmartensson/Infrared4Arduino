// Send nec1 122/27 (volume down for Yamaha) followed by 10 repeats

#include <Nec1Renderer.h>
#include <IrSenderPwm.h>

const IrSignal *irSignal;

void setup() {
    Serial.begin(115200);
    irSignal = Nec1Renderer::newIrSignal(122, 27); // volume_down for Yahama receivers
}

void loop() {
    // Print a textual representation to Serial.
    irSignal->dump(Serial, true);

    // Send it 11 times (payload followed by 10 repeats).
    IrSenderPwm::getInstance(true)->sendIrSignal(*irSignal, 11);

    // Wait 10 seconds.
    delay(10000);
}
