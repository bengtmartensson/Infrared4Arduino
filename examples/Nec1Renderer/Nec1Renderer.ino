// Send nec1 122/27 (volume down for Yamaha) followed by 10 repeats

#include <Nec1Renderer.h>
#include <IrSenderPwm.h>

IrRenderer *renderer;

void setup() {
    Serial.begin(115200);
    renderer = new Nec1Renderer(122, 27); // volume_down for Yahama receivers
}

void loop() {
    renderer->render().dump(Serial, true);
    renderer->send(*IrSenderPwm::getInstance(true), 11);
    delay(10000);
}
