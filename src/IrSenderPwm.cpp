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
    for (unsigned int i = 0; i < irSequence.getLength(); i++) {
        digitalWrite(getOutputPin(), (i & 1) ? LOW : HIGH);
        if (i & 1) {
            TIMER_DISABLE_PWM;
        } else {
            TIMER_ENABLE_PWM;
        }
        delayUSecs(irSequence.getDurations()[i]);
    }
    digitalWrite(getOutputPin(), LOW);
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
    digitalWrite(SEND_PIN, LOW);
    TIMER_CONFIG_KHZ(khz);
}
