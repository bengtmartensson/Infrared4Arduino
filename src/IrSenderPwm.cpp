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
#include "IrSenderPwm.h"
#include <IrTimerDefs.h>

IrSenderPwm *IrSenderPwm::instance = NULL;

IrSenderPwm::IrSenderPwm() : IrSender(SEND_PIN) {
}

void IrSenderPwm::send(const IrSequence& irSequence, frequency_t frequency) {
    enable(frequency/1000);
    unsigned long targetTime = micros();
    for (unsigned int i = 0; i < irSequence.getLength(); i += 2) {
        noInterrupts();
        digitalWriteHigh();
        TIMER_ENABLE_PWM;
        interrupts();
        delayUSecs(irSequence.getDurations()[i]);
        noInterrupts();
        TIMER_DISABLE_PWM;
        digitalWriteLow();
        interrupts();
        targetTime += irSequence.getDurations()[i] + irSequence.getDurations()[i+1];
        delayUSecs((microseconds_t) (targetTime - micros()));
    }
}

IrSenderPwm *IrSenderPwm::newInstance() {
    if (instance != NULL)
        return NULL;
    instance = new IrSenderPwm();
    return instance;
}

IrSenderPwm *IrSenderPwm::getInstance(bool create) {
    if (instance == NULL && create)
        instance = new IrSenderPwm();
    return instance;
}

#ifndef UNUSED
/// @cond false
#define UNUSED
/// @endcond
#endif
void IrSenderPwm::enable(unsigned char khz UNUSED) {
    TIMER_DISABLE_INTR;
    pinMode(SEND_PIN, OUTPUT);
    digitalWriteLow();
    TIMER_CONFIG_KHZ(khz);
}
