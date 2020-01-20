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
#include "InfraredTypes.h"
#include "IrSender.h"

/**
 * IrSender implementation without modulation with inverted output.
 * For example, for emulating an IR receiver with active low output.
 */
class IrSenderNonModInvert : public IrSender {
private:

    inline void writeHigh() {
        digitalWrite(getOutputPin(), LOW);
    }

    inline void writeLow() {
        digitalWrite(getOutputPin(), HIGH);
    }
public:
    IrSenderNonModInvert(pin_t pin);

    /**
     * Two parameter form just to be compatible with the super class.
     * @param irSequence IrSequence to be sent
     * @param frequency MUST be 0
     */
    void send(const IrSequence& irSequence, frequency_t frequency = IrSignal::defaultFrequency) {
        if (frequency == 0U)
            sendNonModulated(irSequence);
    }

    /**
     * Sends the IrSequence as argument.
     * @param irSequence
     */
    void sendNonModulated(const IrSequence& irSequence);
};
