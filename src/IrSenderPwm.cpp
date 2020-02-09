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
#include "boards/boarddefs.h" // for USE_SOFT_CARRIER

#ifdef HAS_HARDWARE_PWM

#include "IrSenderPwm.h"

IrSenderPwm *IrSenderPwm::instance = NULL;

IrSenderPwm::IrSenderPwm() : IrSender(SEND_PIN) {
}

void IrSenderPwm::send(const IrSequence& irSequence, frequency_t frequency) {
    enable(frequency / 1000);
    for (unsigned int i = 0; i < irSequence.getLength(); i++) {
        if (i & 1)
            sendSpace(irSequence.getDurations()[i]);
        else
            sendMark(irSequence.getDurations()[i]);
    }
}

void inline IrSenderPwm::sendMark(milliseconds_t time) {
#ifdef USE_SOFT_CARRIER
    unsigned long start = micros();
    unsigned long stop = start + time;
    if (stop + (unsigned long) periodTime < start)
        // Counter wrap-around, happens very seldomly, but CAN happen.
        // Just give up instead of possibly damaging the hardware.
        return;

    unsigned long nextPeriodEnding = start;
    unsigned long now = micros();
    while (now < stop) {
        SENDPIN_ON(getOutputPin());
        sleepMicros(periodOnTime);
        SENDPIN_OFF(getOutputPin());
        nextPeriodEnding += periodTime;
        sleepUntilMicros(nextPeriodEnding);
        now = micros();
    }
#else
    TIMER_ENABLE_PWM; // supposed to turn on
    delayUSecs(time);
#endif
}

void inline IrSenderPwm::sendSpace(milliseconds_t time) {
#ifdef USE_SOFT_CARRIER
    SENDPIN_OFF(getOutputPin());
#else
    TIMER_DISABLE_PWM;
#endif
    delayUSecs(time);
}

#ifdef USE_SOFT_CARRIER

void inline IrSenderPwm::sleepMicros(unsigned long us) {
#ifdef USE_SPIN_WAIT
    sleepUntilMicros(micros() + us);
#else
    if (us > 0U) // Is this necessary? (Official docu https://www.arduino.cc/en/Reference/DelayMicroseconds does not tell.)
        delayMicroseconds((unsigned int) us);
#endif
}

void inline IrSenderPwm::sleepUntilMicros(unsigned long targetTime) {
#ifdef USE_SPIN_WAIT
    while (micros() < targetTime)
        ;
#else
    sleepMicros(targetTime - micros());
#endif
}
#endif // USE_SOFT_CARRIER

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

void IrSenderPwm::enable(unsigned char khz
#ifndef USE_SOFT_CARRIER
        UNUSED
#endif
        ) {
    TIMER_DISABLE_INTR;
    pinMode(SEND_PIN, OUTPUT);
    SENDPIN_OFF(SEND_PIN);
    TIMER_CONFIG_KHZ(khz);
#ifdef USE_SOFT_CARRIER
    periodTime = (1000U + khz / 2) / khz; // = 1000/khz + 1/2 = round(1000.0/khz)
    periodOnTime = periodTime * DUTY_CYCLE / 100U - PULSE_CORRECTION;

#endif
}

#endif // HAS_HARDWARE_PWM
