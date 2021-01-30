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
#include "IrSender.h"
#include "Board.h"

/**
 * Sending function using timer PWM. Due to the nature of the timers, this is a Highlander,
 * ("There can only be one"), so the class is a singleton class, with private constructor,
 * a factory method that enforces the "highlander property".
 */
class IrSenderPwm : public IrSender {
private:
    static constexpr unsigned int defaultDutyCycle = 50U;
    static IrSenderPwm *instance;

protected:
    IrSenderPwm(pin_t sendPin);
    virtual ~IrSenderPwm() {}

public:
    static unsigned int getDutyCycle() { return defaultDutyCycle; };

    /**
     * Returns a pointer to the instance, or nullptr if not initialized.
     * If argument true, in the latter case creates a new instance and returns it.
     */
    static IrSenderPwm *getInstance(bool create = false, pin_t outputPin = Board::getInstance()->defaultPwmPin());

    /**
     *  Creates a new instance (if not existing) and returns it.
     *  Returns nullptr if an instance already exists.
     */
    static IrSenderPwm *newInstance(pin_t outputPin);

    static void deleteInstance();
};
