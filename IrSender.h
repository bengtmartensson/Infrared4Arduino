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

#ifndef IRSENDER_H
#define	IRSENDER_H

#include "InfraredTypes.h"
#include "IrSignal.h"

/**
 * Common base class for all sending classes.
 */
class IrSender {
private:
    pin_t outputPin;

protected:
    pin_t getOutputPin() const {
        return outputPin;
    }
    void delayUSecs(microseconds_t T);
    IrSender();
    IrSender(pin_t pin);

public:
    virtual ~IrSender();

    //virtual void send(const microseconds_t buf[], unsigned int len, frequency_t frequency) = 0;
    virtual void send(const IrSequence& irSequence, frequency_t frequency = IrSignal::defaultFrequency) = 0;

    void sendIrSignal(const IrSignal& irSignal, unsigned int noSends = 1);

    /** Force output pin inactive. */
    virtual void mute();
};

#endif	/* ! IRSENDER_H */

