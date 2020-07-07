#include "IrSequence.h"
#include "Board.h"
#include <string.h>

IrSequence::IrSequence() : durations(NULL), length(0U), toBeFreed(false) {
};

IrSequence::IrSequence(const microseconds_t *durations_, size_t length_, bool toBeFreed_)
: durations(durations_), length(length_), toBeFreed(toBeFreed_) {
}

IrSequence::IrSequence(const IrSequence& orig) : durations(orig.durations), length(orig.length), toBeFreed(orig.toBeFreed) {
};

IrSequence::IrSequence(const IrSequence& orig, bool toBeFreed_) : durations(orig.durations), length(orig.length), toBeFreed(toBeFreed_) {
};

IrSequence::~IrSequence() {
    if (toBeFreed)
        delete [] durations;
}

const IrSequence IrSequence::emptyInstance;

IrSequence *IrSequence::clone() const {
    microseconds_t *durationsClone = new microseconds_t[length];
    memcpy(durationsClone, durations, length*sizeof(microseconds_t));
    return new IrSequence(durationsClone, length, true);
}

void IrSequence::dump(Stream& stream, bool usingSigns) const {
    for (unsigned int i = 0U; i < length; i++) {
        if (i > 0U)
            stream.print(' ');
        if (usingSigns)
            stream.print((i & 1) ? '-' : '+');
        stream.print(durations[i], DEC);
    }
    stream.println();
}

// If ! HAS_FLASH_READ, allow compiling, but let linking bail out, if using it.
#if HAS_FLASH_READ
IrSequence* IrSequence::readFlash(const microseconds_t *flashData, size_t length) {
    microseconds_t* data = new microseconds_t[length];
    memcpy_PF(data, (uint_farptr_t) flashData, sizeof(microseconds_t) * length);
    return new IrSequence(data, length, true);
}
#endif