#include "IrSequence.h"
#include <string.h>

IrSequence::IrSequence() : durations(nullptr), length(0U), toBeFreed(false) {
};

IrSequence::IrSequence(const microseconds_t *durations, size_t length, boolean toBeFreed)
: durations(durations), length(length), toBeFreed(toBeFreed) {
}

IrSequence::IrSequence(const IrSequence& orig, boolean toBeFreed __attribute__((unused)))
: durations(orig.durations), length(orig.length), toBeFreed(false) {
};

IrSequence::IrSequence(IrSequence& orig, boolean toBeFreed)
: durations(orig.durations), length(orig.length), toBeFreed(toBeFreed && orig.toBeFreed) {
    // If we're taking ownership of the underlying data, relinquish the original
    if (this->toBeFreed)
        orig.toBeFreed = false;
};

IrSequence::~IrSequence() {
    if (toBeFreed)
        delete [] durations;
}

IrSequence *IrSequence::clone() const {
    microseconds_t *durationsClone = new microseconds_t[length];
    memcpy(durationsClone, durations, length*sizeof(microseconds_t));
    return new IrSequence(durationsClone, length, true);
}

void IrSequence::dump(Stream& stream, boolean usingSigns) const {
    for (unsigned int i = 0U; i < length; i++) {
        if (i > 0U)
            stream.print(' ');
        if (usingSigns)
            stream.print((i & 1) ? '-' : '+');
        stream.print(durations[i], DEC);
    }
    stream.println();
}
