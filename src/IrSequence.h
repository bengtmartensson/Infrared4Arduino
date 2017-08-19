#ifndef IRSEQUENCE_H
#define	IRSEQUENCE_H

#include "InfraredTypes.h"

/**
 * This class consists of a vector of durations. The even entries denotes spaces,
 * while the odd entries denotes gaps. The length should always be even, i.e.,
 * the sequences starts with a space, and ends with a gap.
 * This class is immutable.
 */
class IrSequence {
private:
    const microseconds_t *durations;
    size_t length;
    bool toBeFreed;

public:
    /** Create an empty sequence. */
    IrSequence();

    /**
     * Creates an IrSequence.
     * @param durations const array of microseconds durations
     * @param length length of durations. Shuld be even (not checked).
     * @param toBeFreed If true, the destructor will delete the durations array.
     */
    IrSequence(const microseconds_t *durations, size_t length, bool toBeFreed = false);

    virtual ~IrSequence();

    /**
     * Performs shallow copy.
     * @param orig original IrSequence to be cloned
     */
    IrSequence(const IrSequence& orig);

    /**
     * Performs shallow copy.
     * @param orig original IrSequence to be cloned
     * @param toBeFreed If true, the destructor will delete the durations array.
     */
    IrSequence(const IrSequence& orig, bool toBeFreed);

    /**
     * Returns the length of the data.
     * @return length
     */
    size_t getLength() const {
        return length;
    }

    bool isEmpty() const {
        return length == 0;
    }

    const microseconds_t *getDurations() const {
        return durations;
    }

    /**
     * Creates a (deep) clone of the current object.
     * The used must delete it manually.
     * @return pointer to the cloned object
     */
    IrSequence *clone() const;

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
};

#endif	/* IRSEQUENCE_H */

