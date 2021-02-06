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

#pragma once

#include <Arduino.h>
#include "InfraredTypes.h"
#include "IrSender.h"

/**
 * IrSender implementation without modulation, selectively with inverted output.
 * Usage is, for example, controlling an RF transmitter (non-inverting),
 * or for emulating an IR receiver with active low output (inverting).
 */
class IrSenderNonMod : public IrSender {
private:
    bool const invert;
    void enable(frequency_t frequency __attribute__((unused)), dutycycle_t d __attribute__((unused)) = Board::defaultDutyCycle) {};
    void sendSpace(microseconds_t time);
    void sendMark(microseconds_t time);

public:
    IrSenderNonMod(pin_t pin, bool invert = false);

    /**
     * Sends the IrSequence as argument.
     * @param irSequence
     * @param times Number of times to send, default 1.
     */
    void sendNonModulated(const IrSequence& irSequence, unsigned int times = 1U);
};
