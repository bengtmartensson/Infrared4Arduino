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

#include "IrSenderNonMod.h"

IrSenderNonMod::IrSenderNonMod(pin_t pin, bool _invert) : IrSender(pin),invert(_invert) {
}

void IrSenderNonMod::sendNonModulated(const IrSequence& irSequence, unsigned int times) {
    for (unsigned int i = 0; i < times; i++)
        send(irSequence);
}

void IrSenderNonMod::sendSpace(microseconds_t time) {
    if (invert)
        writeHigh();
    else
        writeLow();
    Board::delayMicroseconds(time);
}

void IrSenderNonMod::sendMark(microseconds_t time) {
    if (invert)
        writeLow();
    else
        writeHigh();
    Board::delayMicroseconds(time);
}
