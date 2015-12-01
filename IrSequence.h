#ifndef IRSEQUENCE_H
#define	IRSEQUENCE_H

#include "InfraredTypes.h"

/**
 * This class consists of a vector of durations. The even entries denotes spaces,
 * while the odd entries denotes gaps. The length should always be even, i.e.,
 * the sequences starts with a space, and ends with a gap.
 * This class is immutablle.
 */
class IrSequence {
private:
    const microseconds_t *durations;
    size_t length;
    boolean toBeFreed;

public:
    /** Create an empty sequence. */
    IrSequence();

    /**
     * Creates an IrSequence.
     * @param durations const array of microseconds durations
     * @param length length of durations. Shuld be even (not checked).
     * @param toBeFreed If true, the destructor will delete the durations array.
     */
    IrSequence(const microseconds_t *durations, size_t length, boolean toBeFreed = false);

    virtual ~IrSequence();

    /** Performs shallow copy. */
    IrSequence(const IrSequence& orig);

    IrSequence(const IrSequence& orig, boolean toBeFreed);

    size_t getLength() const {
        return length;
    }

    boolean isEmpty() const {
        return length == 0;
    }

    const microseconds_t *getDurations() const {
        return durations;
    }

    //operator const microseconds_t* () const {
    //    return durations;
    //}

    /**
     * Creates a (deep) clone of the current object.
     * The used must delete it manually.
     * @return pointer to the cloned object
     */
    IrSequence *clone() const;

    /**
     * Prints the IrSequence on the stream provided.
     * @param usingSigns If true,  Gaps are written with a leading '+', spaces with a leading '-'.
     */
    void dump(Stream& stream, boolean usingSigns = false) const;

    /**
     * Prints the IrSequence on the stream provided. Gaps are written with a
     * leading '+', spaces with a leading '-'.
     */
    void dumpWithSigns(Stream& stream) const {
        dump(stream, true);
    };
};

#endif	/* IRSEQUENCE_H */

