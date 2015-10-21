#ifndef IRSIGNAL_H
#define	IRSIGNAL_H

#include <Arduino.h>
#include "InfraredTypes.h"

/**
 * This class models an IR signal with intro-, repeat-, and ending sequences.
 */
class IrSignal {
public:
    IrSignal();
    //IrSignal(const IrSignal& orig);
    virtual ~IrSignal();
    IrSignal(frequency_t frequency, unsigned int lengthIntro, unsigned int lengthRepeat, unsigned int lengthEnding,
            const microseconds_t *intro, const microseconds_t *repeat, const microseconds_t *ending);
private:
    frequency_t frequency; // In Hz
    unsigned int introLength;
    unsigned int repeatLength;
    unsigned int endingLength;
    const microseconds_t *intro;
    const microseconds_t *repeat;
    const microseconds_t *ending;

public:

    frequency_t getFrequency() const {
        return frequency;
    }

    const microseconds_t *getEnding() const {
        return ending;
    }

    const microseconds_t *getRepeat() const {
        return repeat;
    }

    const microseconds_t *getIntro() const {
        return intro;
    }

    unsigned int getEndingLength() const {
        return endingLength;
    }

    unsigned int getRepeaLength() const {
        return repeatLength;
    }

    unsigned int getIntroLength() const {
        return introLength;
    }

    /** Print a human readable representation of the IrSignal on the Steam supplied. */
    void dump(Stream& stream);

    /** Print a human readable representation of the IrSequence on the Stream supplied. */
    static void dump(Stream& stream, unsigned int length, const microseconds_t *data);
};

#endif	/* IRSIGNAL_H */
