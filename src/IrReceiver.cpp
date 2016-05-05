#include "IrReceiver.h"

IrReceiver::IrReceiver(size_t bufSize, pin_t pin_, boolean pullup, microseconds_t me) : IrReader(bufSize) {
    pin = pin_;
    markExcess = me;
    pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
}

void IrReceiver::receive() {
    enable();
    while (!isReady())
        ;
    disable();
}
