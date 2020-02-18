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

#include "IrSenderPwmSoftDelay.h"

IrSenderPwmSoftDelay::IrSenderPwmSoftDelay(pin_t outputPin) : IrSenderPwmSoft(outputPin) {
}

//void IrSenderPwmSoftDelay::sendMark(milliseconds_t time) {
//    unsigned long start = micros();
//    unsigned long stop = start + time;
//    if (stop + (unsigned long) periodTime < start)
//        // Counter wrap-around, happens very seldomly, but CAN happen.
//        // Just give up instead of possibly damaging the hardware.
//        return;
//
//    unsigned long nextPeriodEnding = start;
//    unsigned long now = micros();
//    while (now < stop) {
//        writeHigh();
//        sleepMicros(periodOnTime);
//        writeLow();
//        nextPeriodEnding += periodTime;
//        sleepUntilMicros(nextPeriodEnding);
//        now = micros();
//    }
//}

void IrSenderPwmSoftDelay::sleepMicros(microseconds_t us) {
    if (us > 0U) // Is this necessary? (Official docu https://www.arduino.cc/en/Reference/DelayMicroseconds does not tell.)
        delayMicroseconds((unsigned int) us);
}

void IrSenderPwmSoftDelay::sleepUntilMicros(unsigned long targetTime) {
    sleepMicros(targetTime - micros());
}
