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
#include "InfraredTypes.h"
#include "IrSignal.h"

/**
 * Abstract base class for all sending classes.
 */
class IrSender {
private:
    pin_t sendPin;

protected:
    inline pin_t getSendPin() const {
        return sendPin;
    }

    // TODO: Rewrite for efficiency
    inline void writeHigh() { digitalWrite(sendPin, HIGH); };
    inline void writeLow()  { digitalWrite(sendPin, LOW); };

    void delayUSecs(microseconds_t T);
    IrSender(pin_t pin = SEND_PIN);

    // TODO: something sensible...
    /*virtual*/ static void barfForInvalidPin(pin_t sendPin) { (void) sendPin; };

    virtual void enable(frequency_t frequency) = 0;
    virtual void sendSpace(microseconds_t time) { delayUSecs(time); };
    virtual void sendMark(microseconds_t time) = 0;

public:
    virtual ~IrSender();

    /**
     * Sends an IrSequence with the prescribed frequency
     * @param irSequence
     * @param frequency frequency in Hz
     */
    virtual void send(const IrSequence& irSequence, frequency_t frequency = IrSignal::defaultFrequency);

    /**
     * Sends the IrSignal given as argument the prescribed number of times.
     * This will send the intro sequence signal of the IrSignal, noSend of the
     * repeat, alternatively noSends - 1 repeats if intro is non-empty, and finally
     * the ending sequence.
     * @param irSignal
     * @param noSends
     */
    void sendIrSignal(const IrSignal& irSignal, unsigned int noSends = 1);

    /**
     * Send an IrSignal, when and as long as trigger() returns true.
     * @param irSignal
     * @param trigger Function returning bool.
     */
    void sendWhile(const IrSignal& irSignal, bool(*trigger)());

    /** Force output pin inactive. */
    void mute() { writeLow(); };
};
