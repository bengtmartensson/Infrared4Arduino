#ifndef IRSIGNAL_H
#define	IRSIGNAL_H

#include "InfraredTypes.h"
#include "IrSequence.h"

/**
 * This class models an IR signal with intro-, repeat-, and ending sequences.
 */
class IrSignal {
public:
    static const frequency_t defaultFrequency = 38000U;

    IrSignal();
    IrSignal(const IrSignal& orig);

    virtual ~IrSignal() {
    };
    IrSignal(const microseconds_t *intro, unsigned int lengthIntro,
            const microseconds_t *repeat, unsigned int lengthRepeat,
            const microseconds_t *ending, unsigned int lengthEnding,
            frequency_t frequency = defaultFrequency);

    IrSignal(const microseconds_t *intro, unsigned int lengthIntro,
            const microseconds_t *repeat, unsigned int lengthRepeat,
            frequency_t frequency = defaultFrequency);

    IrSignal(const IrSequence& intro, const IrSequence& repeat, const IrSequence& ending,
            frequency_t frequency = defaultFrequency);

private:
    frequency_t frequency;
    IrSequence intro;
    IrSequence repeat;
    IrSequence ending;

public:

    frequency_t getFrequency() const {
        return frequency;
    }

    const IrSequence& getEnding() const {
        return ending;
    }

    const IrSequence& getRepeat() const {
        return repeat;
    }

    const IrSequence& getIntro() const {
        return intro;
    }

#ifdef ARDUINO
    /**
     * Print a human readable representation of the IrSignal on the Stream supplied.
     * @param usingSigns is true, prepend marks with '+' and gaps with '-'.
     */
    void dump(Stream& stream, boolean usingSigns = false) const;

    /**
     * Print a human readable representation of the IrSignal on the Stream supplied, using signs.
     */
    void dumpWithSigns(Stream& stream) const {
        dump(stream, true);
    };
#endif

    /**
     * Implementation of the count semantics, i.e.,
     * how many repetitions should be sent if the signal is sent noSend times.
     */
    unsigned int noRepetitions(unsigned int noSends) const {
        return getIntro().isEmpty() ? noSends : noSends - 1;
    }
};

#endif	/* IRSIGNAL_H */
