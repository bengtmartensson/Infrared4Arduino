#ifndef IRSEQUENCE_H
#define	IRSEQUENCE_H

#include "InfraredTypes.h"

class IrSequence {
private:
    const microseconds_t *durations;
    unsigned int length;
    boolean toBeFreed;

public:
    /** Create an empty sequence. */
    IrSequence();

    IrSequence(const microseconds_t *durations_, unsigned int length_, boolean toBeFreed = false);

    virtual ~IrSequence();

    /** Performs shallow copy. */
    IrSequence(const IrSequence& orig);

    unsigned int getLength() const {
        return length;
    }

    boolean isEmpty() const {
        return length == 0;
    }

    const microseconds_t *getDurations() const {
        return durations;
    }

#ifdef ARDUINO
    void dump(Stream& stream) const;
#endif
};

#endif	/* IRSEQUENCE_H */

