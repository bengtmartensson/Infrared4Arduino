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

#ifndef IRREADER_H
#define	IRREADER_H
#include <Arduino.h>

#include "InfraredTypes.h"
#include "IrSequence.h"

/**
 * This is an abstract base class for all IR readers, capturing or receiving.
 * It should also serve as an interface that can be printed and decoded.
 */
class IrReader {
public:
    // Defaults
    static const milliseconds_t defaultBeginningTimeout = 2000U;
    static const milliseconds_t defaultEndingTimeout = 30U;
    static const size_t defaultCaptureLength = 100U;

protected:
    milliseconds_t beginningTimeout;
    milliseconds_t endingTimeout;

    size_t bufferSize;

    /** Microseconds subtracted from pulses and added to gaps. May be negative. */
    int16_t markExcess;

    /** True if last receive ended with a timeout */
    boolean timeouted;

    static unsigned int forceEven(unsigned int x) {
        return (x & 1) ? x + 1 : x;
    }

    IrReader(size_t bufSize_) : bufferSize(forceEven(bufSize_)),timeouted(false) {
    }

    IrReader() {
    }

    virtual ~IrReader() {
    };

public:
    virtual void reset() {
        timeouted = false;
    };

    virtual void enable() {
    };

    virtual void disable() {
    };

    virtual void receive() = 0;

    virtual boolean isReady() const = 0;

    virtual size_t getDataLength() const = 0; // was getCaptureCount())
    virtual microseconds_t getDuration(unsigned int index) const = 0;
    virtual void dump(Stream &stream) const;
    IrSequence *toIrSequence() const;

    virtual boolean isEmpty() const { // was hasContent())
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

    void setMarkExcess(int16_t markExcess_) {
        markExcess = markExcess_;
    }

    int16_t getMarkExcess() const {
        return markExcess;
    }
};

#endif	/* IRCAPTURER_H */
