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

#include "IrReader.h"

// Cannot use IrSequence.dump directly!
void IrReader::dump(Stream &stream) const {
    size_t count = getDataLength();
    for (unsigned int i = 0U; i < count; i++) {
        if (i > 0U)
            stream.print(" ");
        stream.print((i & 1U) ? '-' : '+');
        stream.print(getDuration(i), DEC);
    }
}

IrSequence *IrReader::toIrSequence() const {
    microseconds_t *durations = new microseconds_t[getDataLength()];
    for (unsigned int i = 0; i < getDataLength(); i++)
        durations[i] = getDuration(i);
    return new IrSequence(durations, getDataLength());
}
