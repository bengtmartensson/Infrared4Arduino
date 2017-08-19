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

#ifndef IRSENDERPWM_H
#define	IRSENDERPWM_H

#include <Arduino.h>
#include "IrSender.h"

/**
 * Sending function using timer PWM. Due to the nature of the timers, this is a Highlander,
 * ("There can only be one"), so the class is a singleton class, with private constructor,
 * a factory method that enforces the "highlander property".
 */
class IrSenderPwm : public IrSender {
private:
    IrSenderPwm();
    ~IrSenderPwm() {}
    static IrSenderPwm *instance;
    void enable(unsigned char khz);

public:
    /**
     * Returns a pointer to the instance, or NULL if not initialized.
     * If argument true, in the latter case creates a new instance and returns it.
     */
    static IrSenderPwm *getInstance(bool create = false);

    /**
     *  Creates a new instance (if not existing) and returns it.
     *  Returns NULL if an instance already exists.
     */
    static IrSenderPwm *newInstance();

    static void deleteInstance() {
        delete instance;
        instance = NULL;
    }

    void send(const IrSequence& sequence, frequency_t frequency = IrSignal::defaultFrequency);
};

#endif	/* IRSENDERPWM_H */

