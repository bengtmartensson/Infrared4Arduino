#include "IrSequence.h"

IrSequence::IrSequence() {
    durations = NULL;
    length = 0U;
    toBeFreed = false;
};

IrSequence::IrSequence(const microseconds_t *durations_, unsigned int length_, boolean toBeFreed_) {
    durations = durations_;
    length = length_;
    toBeFreed = toBeFreed_;
};

IrSequence::IrSequence(const IrSequence& orig) {
    durations = orig.durations;
    length = orig.length;
    toBeFreed = false;
};

IrSequence::~IrSequence() {
    if (toBeFreed)
        delete [] durations;
}

#ifdef ARDUINO
void IrSequence::dump(Stream& stream) const {
    for (unsigned int i = 0U; i < length; i++) {
        if (i > 0U)
            stream.print(' ');
        stream.print((i & 1) ? '-' : '+');
        stream.print(durations[i], DEC);
    }
    stream.println();
}
#endif
