#pragma once

#include "IrSignal.h"
#include "IrReader.h"

/**
 * This class packs an IrSequence into a dummy, immutable IrReader.
 * It is basically intended for debugging and such.
 */
class IrSequenceReader : public IrReader {
private:
    IrSequence irSequence;

public:
    IrSequenceReader() : irSequence() {
    };

    IrSequenceReader(const IrSequenceReader& orig) : IrReader(),irSequence(orig.irSequence) {
    };

    IrSequenceReader(IrSequenceReader&& orig) : IrReader(),irSequence(orig.irSequence) {
    };

    IrSequenceReader(const IrSequence& irSequence_) : IrReader(),irSequence(irSequence_) {
    };

    virtual ~IrSequenceReader() {
    };

    virtual frequency_t getFrequency() const {
        return IrSignal::defaultFrequency;
    };

    void receive() {
    };

    bool isReady() const {
        return true;
    };

    size_t getDataLength() const {
        return irSequence.getLength();
    };

    microseconds_t getDuration(unsigned int index) const {
        return irSequence[index];
    };
};
