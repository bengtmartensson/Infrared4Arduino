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
#include "IrSender.h"

/**
 *
 */
class IrSenderSoftCarrier : public IrSender {
private:

    inline void writeHigh() {
        digitalWrite(getOutputPin(), HIGH);
    }

    inline void writeLow() {
        digitalWrite(getOutputPin(), LOW);
    }

    void enable(frequency_t khz);
    void mark(microseconds_t duration);

    microseconds_t periodTime;
    microseconds_t periodOnTime;
    microseconds_t periodOffTime;

    static const unsigned int dutyCycle = 40;
    //static const int pulseCorrection = 3;

public:

    IrSenderSoftCarrier(pin_t sendPin);
    ~IrSenderSoftCarrier() {}

    void send(const IrSequence& sequence, frequency_t frequency = IrSignal::defaultFrequency);
};
