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
#include "boards/boarddefs.h" // for HAS_HARDWARE_PWM

#ifdef HAS_HARDWARE_PWM

#include "IrSenderPwmHard.h"
#include "IrReceiver.h"

IrSenderPwmHard *IrSenderPwmHard::instance = NULL;

IrSenderPwmHard::IrSenderPwmHard(pin_t outputPin) : IrSenderPwm(outputPin) {
};

IrSenderPwmHard::~IrSenderPwmHard() {
};

IrSenderPwmHard *IrSenderPwmHard::newInstance(pin_t outputPin) {
    if (instance != NULL)
        return NULL;
    instance = new IrSenderPwmHard(outputPin);
    return instance;
}

IrSenderPwmHard *IrSenderPwmHard::getInstance(bool create, pin_t outputPin) {
    if (instance == NULL && create)
        instance = new IrSenderPwmHard(outputPin);
    return instance;
}

#ifndef UNUSED
/// @cond false
#define UNUSED
/// @endcond
#endif

void IrSenderPwmHard::enable(frequency_t frequency UNUSED) {
    TIMER_DISABLE_INTR;
    //pinMode(getPin(), OUTPUT);
    writeLow();
    TIMER_CONFIG_KHZ(frequency);
}

void inline IrSenderPwmHard::sendMark(milliseconds_t time) {
    TIMER_ENABLE_PWM; // supposed to turn on
    delayUSecs(time);
    TIMER_DISABLE_PWM;
}

#endif // HAS_HARDWARE_PWM
