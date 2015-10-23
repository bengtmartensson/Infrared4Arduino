/*
Copyright (C) 2015 Bengt Martensson.

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

#ifndef IRSENDERNONMOD_H
#define	IRSENDERNONMOD_H

#include <Arduino.h>
#include "InfraredTypes.h"
#include "IrSender.h"

class IrSenderNonMod : public IrSender {
public:
    IrSenderNonMod(pin_t pin);

    /** Three parameter form just to be compatible with the super class */
    void send(const microseconds_t buf[], unsigned int len, frequency_t frequency = 0) {
        if (frequency == 0U)
            sendNonModulated(buf, len);
    }

    void sendNonModulated(const microseconds_t buf[], unsigned int len);
};

#endif	/* ! IRSENDERNONMOD_H */

