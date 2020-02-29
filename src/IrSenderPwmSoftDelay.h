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

#pragma once

#include <Arduino.h>
#include "IrSenderPwmSoft.h"

/**
 * Sending function using timer PWM. Due to the nature of the timers, this is a Highlander,
 * ("There can only be one"), so the class is a singleton class, with private constructor,
 * a factory method that enforces the "highlander property".
 */
class IrSenderPwmSoftDelay : public IrSenderPwmSoft {
public:
    IrSenderPwmSoftDelay(pin_t outpitPin); // default is not meaningful!!

    virtual ~IrSenderPwmSoftDelay() {
    }

private:
    void sleepMicros(microseconds_t us);
    void sleepUntilMicros(uint32_t terminateTime);
};
