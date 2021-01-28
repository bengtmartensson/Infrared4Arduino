/*
Copyright (C) 2015 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#include <Arduino.h>
#include "Board.h"

#ifdef HAS_HARDWARE_PWM
#include "IrSenderPwmHard.h"

IrSenderPwmHard *IrSenderPwmHard::instance = nullptr;

IrSenderPwmHard::IrSenderPwmHard(pin_t outputPin __attribute__((unused))) : IrSenderPwm(PWM_PIN) {
}

IrSenderPwmHard::~IrSenderPwmHard() {
    disable();
}

IrSenderPwmHard *IrSenderPwmHard::newInstance(pin_t outputPin) {
    if (instance != nullptr)
        return nullptr;
    instance = new IrSenderPwmHard(outputPin);
    return instance;
}

IrSenderPwmHard *IrSenderPwmHard::getInstance(bool create, pin_t outputPin) {
    if (instance == nullptr && create)
        instance = new IrSenderPwmHard(outputPin);
    return instance;
}

void IrSenderPwmHard::enable(frequency_t frequency, dutycycle_t dutyCycle) {
    Board::getInstance()->enablePwm(getPin(), frequency, dutyCycle);
}

void IrSenderPwmHard::disable() {
    Board::getInstance()->disablePwm();
}

void inline IrSenderPwmHard::sendMark(milliseconds_t time) {
    Board::getInstance()->sendPwmMark(time);
}

#endif // HAS_HARDWARE_PWM
