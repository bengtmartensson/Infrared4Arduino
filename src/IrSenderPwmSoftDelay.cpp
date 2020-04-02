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

void inline IrSenderPwmSoftDelay::sleepMicros(microseconds_t us) {
    if (us > 0U) // Is this necessary? (Official docu https://www.arduino.cc/en/Reference/DelayMicroseconds does not tell.)
        Board::delayMicroseconds(us);
}

void inline IrSenderPwmSoftDelay::sleepUntilMicros(uint32_t targetTime) {
    int32_t time = targetTime - micros();
    if (time < 0)
        return;
    delay(time/1000UL);
    int32_t rest = targetTime - micros();
    if (rest >= 4)
    ::delayMicroseconds(rest);
}
