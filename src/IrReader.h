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
#include "IrSequence.h"

/**
 * Abstract base class for all IR readers, capturing or receiving.
 * It should also serve as an interface description,
 * allowing for received data to be printed and decoded.
 */
class IrReader {
public:
    // Defaults
    static constexpr milliseconds_t defaultBeginningTimeout = 2000U;
    static constexpr milliseconds_t defaultEndingTimeout = 30U;
    static constexpr size_t defaultCaptureLength = 100U;

protected:
    milliseconds_t beginningTimeout;
    milliseconds_t endingTimeout;

    size_t bufferSize;

    /** Microseconds subtracted from pulses and added to gaps. May be negative. */
    int16_t markExcess;

    /** True if last receive ended with a timeout */
    bool timeouted;

    static unsigned int forceEven(unsigned int x) {
        return (x & 1) ? x + 1 : x;
    }

    /**
     * Constructs an IrReader with buffersize bufSize_, possibly increased to be even.
     * @param bufSize_
     */
    IrReader(size_t bufSize_) : bufferSize(forceEven(bufSize_)),timeouted(false) {
    }

    IrReader() {
    }

#ifndef DOXYGEN
    IrReader(const IrReader&) = delete;
    IrReader(IrReader&&) = delete;
    IrReader& operator=(const IrReader&) = delete;
    IrReader& operator=(IrReader&&) = delete;
#endif // ! DOXYGEN

    virtual ~IrReader() {
    };

public:
    virtual void reset() {
        timeouted = false;
    };

    /**
     * Returns frequency of received signal.
     * This is a dumb default implementation, to be overridden when meaningful.
     * @return frequency
     */
    virtual frequency_t getFrequency() const = 0;

    /**
     * Start reception of IR data.
     */
    virtual void enable() {
    };

    /**
     * Stop reception of IR data.
     */
    virtual void disable() {
    };

    /**
     * Convenience function: enable, wait until data is collected or timeout has occured, disable.
     */
    virtual void receive() = 0;

    /**
     * Returns true if there is collected data.
     * @return status
     */
    virtual bool isReady() const = 0;

    /**
     * Same as isReady()
     * @return
     */
    operator bool() const {
        return isReady();
    }

    /**
     * Returns the number of collected durations.
     * @return number durations
     */
    virtual size_t getDataLength() const = 0;

    /**
     * Returns the index-th duration, if possible.
     * @param index index of duration
     * @return requested duration
     */
    virtual microseconds_t getDuration(unsigned int index) const = 0;

    /**
     * Prints a textual representation of the received data to the Stream supplied.
     * @param stream Stream to which to print
     */
    virtual void dump(Stream &stream) const;

    /**
     * Generates an IrSequence from the IrReader.
     * @return IrSequence. The user must delete this to avoid memory leaks.
     */
    IrSequence *toIrSequence() const;

    virtual bool isEmpty() const {
        return getDataLength() == 0;
    }

    virtual void setEndingTimeout(milliseconds_t timeOut) {
        endingTimeout = timeOut;
    }

    virtual milliseconds_t getEndingTimeout() const {
        return endingTimeout;
    }

    virtual void setBeginningTimeout(milliseconds_t timeOut) {
        beginningTimeout = timeOut;
    }

    virtual milliseconds_t getBeginningTimeout() const {
        return beginningTimeout;
    }

    unsigned int getBufferSize() const {
        return bufferSize;
    }

    /**
     * Sets the markExcess, a number (possibly negative) to be subtracted from the on-durations
     * and added to the off.durations.
     * @param markExcess_ possibly negative new value
     */
    void setMarkExcess(int16_t markExcess_) {
        markExcess = markExcess_;
    }

    /**
     * Gets the markExcess, a number (possibly negative) to be subtracted from the on-durations
     * and added to the off.durations.
     * @return markExcess
     */
    int16_t getMarkExcess() const {
        return markExcess;
    }
};
