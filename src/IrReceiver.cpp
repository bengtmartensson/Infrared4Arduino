#include "IrReceiver.h"

IrReceiver::IrReceiver(size_t bufSize, pin_t pin_, bool pullup, microseconds_t me) : IrReader(bufSize) {
    pin = pin_;
    markExcess = me;
    Board::getInstance()->setPinMode(pin, pullup ? INPUT_PULLUP : INPUT);
}

void IrReceiver::receive() {
    enable();
    while (!isReady())
        ;
    disable();
}
