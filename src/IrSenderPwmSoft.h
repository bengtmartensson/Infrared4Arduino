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
#include "IrSenderPwm.h"

/**
 * Sending function using timer PWM. Due to the nature of the timers, this is a Highlander,
 * ("There can only be one"), so the class is a singleton class, with private constructor,
 * a factory method that enforces the "highlander property".
 */
class IrSenderPwmSoft : public IrSenderPwm {
protected:
    IrSenderPwmSoft(pin_t outputPin); // no default!
    virtual ~IrSenderPwmSoft() {}
    void enable(frequency_t hz, dutycycle_t dutyCycle = Board::defaultDutyCycle);
    void sendMark(microseconds_t time);
    static constexpr unsigned int PULSE_CORRECTION = 3U;

    virtual void sleepMicros(microseconds_t us) = 0;
    virtual void sleepUntilMicros(uint32_t terminateTime) = 0;
    microseconds_t periodTime;
    microseconds_t periodOnTime;
    microseconds_t periodOffTime;
};
