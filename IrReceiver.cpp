#include "IrReceiver.h"

IrReceiver::IrReceiver(unsigned int bufSize, pin_t pin_, boolean pullup, microseconds_t me) : IrReader(bufSize) {
    pin = pin_;
    markExcess = me;
    pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
}
