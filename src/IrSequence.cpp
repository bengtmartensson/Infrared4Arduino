#include "IrSequence.h"
#include "Board.h"
#include <string.h>

IrSequence::IrSequence(const microseconds_t *durations_, size_t length_) : durations(durations_),length(length_) {
}

IrSequence::IrSequence(IrSequence&& orig) : durations(orig.durations),length(orig.length) {
    orig.durations = nullptr;
    orig.length = 0U;
}

IrSequence::IrSequence(const IrSequence& orig) : length(orig.length) {
    microseconds_t* data = new microseconds_t[orig.length];
    for (unsigned int i = 0U; i < orig.length; i++)
        data[i] = orig.durations[i];
    durations = data;
}

IrSequence& IrSequence::operator=(IrSequence&& rhs) {
    if (this != &rhs) {
        delete [] durations;
        durations = rhs.durations;
        length = rhs.length;
        rhs.durations = nullptr;
        rhs.length = 0;
    }
    return *this;
}

IrSequence& IrSequence::operator=(const IrSequence& rhs) {
    delete [] durations;
    microseconds_t* data = new microseconds_t[rhs.length];
    for (unsigned int i = 0U; i < rhs.length; i++)
        data[i] = rhs.durations[i];
    durations = data;
    length = rhs.length;
    return *this;
}

IrSequence::~IrSequence() {
    delete [] durations;
}

const IrSequence IrSequence::emptyInstance;

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
    return new IrSequence(data, length);
}
#endif