/*
Copyright (C) 2019 Bengt Martensson.

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

#pragma once

#include <Arduino.h>
#include "IrSenderPwmSoft.h"

/**
 * @class IrSenderPwmSpinWait
 *
 * This sender class generates the modulation in software, using spin wait.
 * It does not use any timer, not even for delay().
 * It will therefore run on all sufficiently powerful hardware.
 */
class IrSenderPwmSpinWait : public IrSenderPwmSoft {
private:

//    inline void writeHigh() {
//        digitalWrite(getOutputPin(), HIGH);
//    }
//
//    inline void writeLow() {
//        digitalWrite(getOutputPin(), LOW);
//    }

    //void enable(frequency_t khz);
//    void mark(microseconds_t duration);

//    microseconds_t periodTime;
//    microseconds_t periodOnTime;
//    microseconds_t periodOffTime;

    //static const unsigned int dutyCycle = 40;
    //static const int pulseCorrection = 3;

    //void sendSpace(microseconds_t t);
    //void sendMark(microseconds_t t);
    void sleepMicros(microseconds_t t);
    void sleepUntilMicros(unsigned long t);

public:

    IrSenderPwmSpinWait(pin_t sendPin);
    virtual ~IrSenderPwmSpinWait() {};
};
