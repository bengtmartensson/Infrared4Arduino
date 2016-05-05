#ifndef IRSEQUENCEREADER_H
#define	IRSEQUENCEREADER_H

#include "IrSequence.h"
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

    IrSequenceReader(const IrSequence& irSequence_) : IrReader(),irSequence(irSequence_) {
    };

    virtual ~IrSequenceReader() {
    };

    void receive() {
    };

    boolean isReady() const {
        return true;
    };

    size_t getDataLength() const {
        return irSequence.getLength();
    };

    microseconds_t getDuration(unsigned int index) const {
        return irSequence.getDurations()[index];
    };
};

#endif	/* IRSEQUENCEREADER_H */
