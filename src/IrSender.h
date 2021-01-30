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
#include "IrSignal.h"
#include "Board.h"

/**
 * Abstract base class for all sending classes.
 */
class IrSender {
private:
    pin_t sendPin;

public:
    inline pin_t getPin() const {
        return sendPin;
    }

protected:
    // TODO: Rewrite for efficiency
    inline void writeHigh() { Board::getInstance()->writeHigh(sendPin); };
    inline void writeLow()  { Board::getInstance()->writeLow(sendPin); };

    IrSender(pin_t pin);

    // TODO: something sensible...
    /*virtual*/ static void barfForInvalidPin(pin_t sendPin __attribute__((unused))) {};

    virtual void enable(frequency_t frequency, dutycycle_t dutyCycle = Board::defaultDutyCycle) = 0;
    virtual void sendSpace(microseconds_t time) { Board::delayMicroseconds(time); };
    virtual void sendMark(microseconds_t time) = 0;

public:
#ifndef DOXYGEN
    IrSender() = delete;
    IrSender(const IrSender&) = delete;
    IrSender(IrSender&&) = delete;
    IrSender& operator=(const IrSender&) = delete;
    IrSender& operator=(IrSender&&) = delete;
#endif // ! DOXYGEN

    virtual ~IrSender();

    /**
     * Sends an IrSequence with the prescribed frequency
     * @param irSequence
     * @param frequency frequency in Hz
     * @param dutyCycle
     */
    virtual void send(const IrSequence& irSequence, frequency_t frequency = IrSignal::defaultFrequency, dutycycle_t dutyCycle = Board::defaultDutyCycle);

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
