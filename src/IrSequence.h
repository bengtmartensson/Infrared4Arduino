#pragma once

#include "InfraredTypes.h"

/**
 * This class consists of a vector of durations. The even entries denotes spaces,
 * while the odd entries denotes gaps. The length should always be even, i.e.,
 * the sequences starts with a space, and ends with a gap.
 * This class is immutable (except for assignments).
 */
class IrSequence {
private:
    const microseconds_t *durations = nullptr;
    size_t length = 0U;

public:
    /** Create an empty sequence. */
    IrSequence() {};

    /**
     * Creates an IrSequence.
     * @param durations const array of microseconds durations. Will be "moved", and delete []-d by the destructor.
     * @param length length of durations. Should be even (not checked).
     */
    IrSequence(const microseconds_t *durations, size_t length);

    virtual ~IrSequence();

    /**
     * Copy constructor.
     * @param orig original IrSequence to be copied.
     */
    IrSequence(const IrSequence& orig);

    /**
     * Move constructor.
     * @param orig original IrSequence to be moved
     */
    IrSequence(IrSequence&& orig);

    /**
     * Copy assignment.
     * @param rhs
     */
    IrSequence& operator=(const IrSequence& rhs);

    /**
     * Move assignment.
     * @param rhs
     */
    IrSequence& operator=(IrSequence&& rhs);

    static const IrSequence emptyInstance;

    /**
     * True if and only if non-empty.
     * @return
     */
    operator bool() const { return ! isEmpty(); }

    /**
     * Returns the number of durations.
     * @return length
     */
    size_t getLength() const {
        return length;
    }

    bool isEmpty() const {
        return length == 0U;
    }

    const microseconds_t *getDurations() const {
        return durations;
    }

    size_t size() const {
        return length;
    }

    const microseconds_t* begin() const {
        return durations;
    }

    const microseconds_t* end() const {
        return durations + length;
    }

    microseconds_t operator[](int i) const {
        return durations[i];
    }

    /**
     * Prints the IrSequence on the stream provided.
     * @param stream Stream onto the output is printed.
     * @param usingSigns If true,  Gaps are written with a leading '+', spaces with a leading '-'.
     */
    void dump(Stream& stream, bool usingSigns = false) const;

    /**
     * Prints the IrSequence on the stream provided. Gaps are written with a
     * leading '+', spaces with a leading '-'.
     * @param stream Stream onto the output is printed.
     */
    void dumpWithSigns(Stream& stream) const {
        dump(stream, true);
    };

    // Do NOT put #ifdef HAS_FLASH_READ here, leads to circular includes.
    /**
     * Create an IrSequence from data in PROGMEM.
     * This must manually be deleted by the programmer.
     * The function is available only on platforms implementing the memcpy_PF
     * call.
     * @param flashData
     * @param length
     * @return Pointer to IrSequence.
     */
    static IrSequence* readFlash(const microseconds_t *flashData, size_t length);
};
