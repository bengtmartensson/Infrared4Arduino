#ifndef IRSIGNAL_H
#define	IRSIGNAL_H

#include "InfraredTypes.h"
#include "IrSequence.h"

/**
 * This class models an IR signal with intro-, repeat-, and ending sequences.
 * This class is immutable.
 */
class IrSignal {
public:
    static const frequency_t defaultFrequency = 38000U;
    static const frequency_t invalidFrequency = (frequency_t) -1;

    IrSignal();
    IrSignal(const IrSignal& orig);

    virtual ~IrSignal();
    IrSignal(const microseconds_t *intro, size_t lengthIntro,
            const microseconds_t *repeat, size_t lengthRepeat,
            const microseconds_t *ending, size_t lengthEnding,
            frequency_t frequency = defaultFrequency,
            bool toBeFreed = false);

    IrSignal(const microseconds_t *intro, size_t lengthIntro,
            const microseconds_t *repeat, size_t lengthRepeat,
            frequency_t frequency = defaultFrequency,
            bool toBeFreed = false);

    IrSignal(const IrSequence& intro, const IrSequence& repeat, const IrSequence& ending,
            frequency_t frequency, bool toBeFreed);

    IrSignal(const IrSequence& intro, const IrSequence& repeat, const IrSequence& ending,
            frequency_t frequency = defaultFrequency);

private:
    const frequency_t frequency;
    const IrSequence intro;
    const IrSequence repeat;
    const IrSequence ending;

public:

    /**
     * Creates a (deep) copy of the current object.
     * The user must delete it manually.
     * @return Pointer to the cloned object.
     */
    IrSignal *clone() const;

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

    /**
     * Print a human readable representation of the IrSignal on the Stream supplied.
     * @param stream Stream onto the output is printed.
     * @param usingSigns is true, prepend marks with '+' and gaps with '-'.
     */
    void dump(Stream& stream, bool usingSigns = false) const;

    /**
     * Print a human readable representation of the IrSignal on the Stream supplied, using signs.
     * @param stream Stream onto the output is printed.
     */
    void dumpWithSigns(Stream& stream) const {
        dump(stream, true);
    };

    /**
     * If the frequency is sensible, print it to the stream and return true.
     * Otherwise do nothing and return false.
     * No extra spaces or line feeds are generated.
     */
    bool dumpFrequency(Stream& stream) const {
        return dumpFrequency(stream, frequency);
    };

    /**
     * Static version of dumpFrequency.
     * @param stream Stream onto the output is printed.
     * @param frequency modulation frequency
     */
    static bool dumpFrequency(Stream& stream, frequency_t frequency);

    /**
     * Implementation of the count semantics, i.e.,
     * how many repetitions should be sent if the signal is sent noSend times.
     * @param noSends number of times to "send signal".
     */
    unsigned int noRepetitions(unsigned int noSends) const {
        return noSends == 0 ? 0
                : getIntro().isEmpty() ? noSends : noSends - 1;
    }


};

#endif	/* IRSIGNAL_H */
