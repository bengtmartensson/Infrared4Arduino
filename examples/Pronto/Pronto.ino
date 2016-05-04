// Sends a signal in Pronto format

#include <IrSenderPwm.h>
#include <Pronto.h>

IrSignal *irSignal;

void setup() {
    // NEC1 122 29, power on for many Yamaha amplifiers.
    irSignal = Pronto::parse("0000 006C 0022 0002 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 015B 0057 0016 0E6C");
}

void loop() {
    IrSenderPwm::getInstance(true)->sendIrSignal(*irSignal);
    delay(5000);
}

