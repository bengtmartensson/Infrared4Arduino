/*
Copyright (C) 2020 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
 */

#include "Board.h"

/**
 * Version of delayMicroseconds able to grok more than 16383 micros.
 * @param t delay time in microseconds_t
 */
// See https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
void Board::delayMicroseconds(microseconds_t t) {
    if (t) {
        if (t > 16383U) {
            ::delayMicroseconds(t % 1000U);
            delay(t / 1000U);
        } else
            ::delayMicroseconds(t);
    }
}

Board* Board::instance = new CURRENT_CLASS();
