/*
Copyright (C) 2020 Bengt Martensson.

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
#include "IrSenderPwmSpinWait.h"

//#define wait(t) delayMicroseconds(t)
//inline void wait(unsigned long t) {
//    unsigned long end = micros() + t;
//    while (micros() < end)
//#if ! defined(ARDUINO) && ! defined(REAL_TIME)
//        // increment the simulated time, otherwise will loop forever
//        delayMicroseconds(1)
//#endif
//        ;
//}

IrSenderPwmSpinWait::IrSenderPwmSpinWait(pin_t sendPin) : IrSenderPwmSoft(sendPin) {
}

//void IrSenderPwmSpinWait::send(const IrSequence& irSequence, frequency_t frequency) {
//    enable(frequency);
//    unsigned long start = micros();
//    for (unsigned int i = 0; i < irSequence.getLength(); i += 2) {
//        mark(irSequence.getDurations()[i]); // leaves LOW
//        unsigned long ending = start + irSequence.getDurations()[i] + irSequence.getDurations()[i + 1];
//        milliseconds_t remain = ending > start /* not overflow */ ? (microseconds_t) (ending - micros()) : irSequence.getDurations()[i + 1];
//        wait(remain);
//        start = ending;
//    }
//}

void inline IrSenderPwmSpinWait::sleepMicros(microseconds_t us) {
    sleepUntilMicros(micros() + us);
}

void inline IrSenderPwmSpinWait::sleepUntilMicros(unsigned long targetTime) {
    while (micros() < targetTime)
#if ! defined(ARDUINO) && ! defined(REAL_TIME)
        // increment the simulated time, otherwise will loop forever
        delayMicroseconds(1)
#endif
        ;
}

//+=============================================================================
// Sends an IR mark for the specified number of microseconds.
// The mark output is modulated at the PWM frequency.
//
//void IrSenderPwmSpinWait::sendMark(microseconds_t time) {
//    unsigned long start = micros();
//    unsigned long stop = start + time;
//    if (stop + periodTime < start)
//        // Counter wrap-around, happens very seldomly, but CAN happen.
//        // Just give up instead of possibly damaging the hardware.
//        return;
//
//    //unsigned long nextPeriodEnding = start;
//    unsigned long now = micros();
//    while (now < stop) {
//        unsigned long then = now + periodTime;
//        writeHigh();
//        wait(periodOnTime);
//        writeLow();
//        //nextPeriodEnding += periodTime;
//        sleepUntilMicros(then);
//        now = micros();
//    }
//}

//+=============================================================================
// Leave pin off for time (given in microseconds)
// Sends an IR space for the specified number of microseconds.
// A space is no output, so the PWM output is disabled.
//
//
//void IrSenderPwmSpinWait::space(unsigned int time) {
//    TIMER_DISABLE_PWM; // Disable pin 3 PWM output
//    if (time > 0) IrSenderPwmSpinWait::custom_delay_usec(time);
//}

//+=============================================================================
// Custom delay function that circumvents Arduino's delayMicroseconds limit

//void IrSenderPwmSpinWait::custom_delay_usec(unsigned long uSecs) {
//    if (uSecs > 4) {
//        unsigned long start = micros();
//        unsigned long endMicros = start + uSecs - 4;
//        if (endMicros < start) { // Check if overflow
//            while (micros() > start) {
//            } // wait until overflow
//        }
//        while (micros() < endMicros) {
//        } // normal wait
//    }
//    //else {
//    //  __asm__("nop\n\t"); // must have or compiler optimizes out
//    //}
//}
