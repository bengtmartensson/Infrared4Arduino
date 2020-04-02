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
#include "IrSenderPwmSoft.h"

IrSenderPwmSoft::IrSenderPwmSoft(pin_t outputPin) : IrSenderPwm(outputPin) {
}

//void IrSenderPwmSoft::sendSpace(milliseconds_t time) {
//    writeLow();
//    delayUSecs(time);
//}

//void inline IrSenderPwmSoft::sleepMicros(unsigned long us) {
//#ifdef USE_SPIN_WAIT
//    sleepUntilMicros(micros() + us);
//#else
//    if (us > 0U) // Is this necessary? (Official docu https://www.arduino.cc/en/Reference/DelayMicroseconds does not tell.)
//        delayMicroseconds((unsigned int) us);
//#endif
//}

//void IrSenderPwmSoft::sleepUntilMicros(unsigned long targetTime) {
//#ifdef USE_SPIN_WAIT
//    while (micros() < targetTime)
//        ;
//#else
//    sleepMicros(targetTime - micros());
//#endif
//}

void IrSenderPwmSoft::enable(frequency_t hz, dutycycle_t dutyCycle) {
    writeLow();
    periodTime = (1000000UL + hz / 2L) / hz; // = 1000000/hz + 1/2 = round(1000000.0/hz)
    periodOnTime = periodTime * dutyCycle / 100U /*- pulseCorrection*/;
    periodOffTime = periodTime - periodOnTime;
}

void IrSenderPwmSoft::sendMark(microseconds_t time) {
    unsigned long start = micros();
    unsigned long stop = start + time;
    if (stop + periodTime < start)
        // Counter wrap-around, happens very seldomly, but CAN happen.
        // Just give up instead of possibly damaging the hardware.
        return;

    //unsigned long nextPeriodEnding = start;
    unsigned long now = micros();
    while (now < stop) {
        unsigned long then = now + periodTime;
        writeHigh();
        sleepMicros(periodOnTime);
        writeLow();
        //nextPeriodEnding += periodTime;
        sleepUntilMicros(then);
        now = micros();
    }
}