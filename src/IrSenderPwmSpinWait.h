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
public:
    IrSenderPwmSpinWait(pin_t sendPin);

    virtual ~IrSenderPwmSpinWait() {
    };

private:
    void sleepMicros(microseconds_t t);
    void sleepUntilMicros(uint32_t t);
};
