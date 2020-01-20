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

#include "IrSenderNonModInvert.h"

IrSenderNonModInvert::IrSenderNonModInvert(pin_t pin) : IrSender(pin) {
    writeLow();
}

void IrSenderNonModInvert::sendNonModulated(const IrSequence &irSequence) {
    for (unsigned int i = 0; i < irSequence.getLength(); i += 2) {
        writeHigh();
        delayUSecs(irSequence.getDurations()[i]);
        writeLow();
        delayUSecs(irSequence.getDurations()[i+1]);
    }
    //digitalWrite(getOutputPin(), LOW);
}
