#pragma once

#include "InfraredTypes.h"
#include "IrSequence.h"
#include "version.h"

/**
 * This class models an IR signal with intro-, repeat-, and ending sequences.
 * This class is immutable.
 */
class IrSignal {
public:
    static constexpr frequency_t defaultFrequency = 38000U;
    static constexpr frequency_t invalidFrequency = static_cast<frequency_t>(-1);
    static constexpr dutycycle_t noDutyCycle = static_cast<dutycycle_t>(-1);
    static constexpr dutycycle_t defaultDutyCycle = noDutyCycle;
    static constexpr const char *version = VERSION;

private:
    // Maintainer note: the members being const prohibits assignment operators.
    // Alternatively, the members can be made non-const and assignments allowed.
    const frequency_t frequency = defaultFrequency;
    const dutycycle_t dutyCycle = defaultDutyCycle;
    const IrSequence intro;
    const IrSequence repeat;
    const IrSequence ending;

public:
    /**
     * Constructs an empty IrSignal.
     */
    IrSignal() {};
    IrSignal(const IrSignal& orig) = default;
    IrSignal(IrSignal&& orig) = default;
    IrSignal& operator=(const IrSignal& rhs) = default;
    IrSignal& operator=(IrSignal&& rhs) = default;

    virtual ~IrSignal() = default;

    /**
     * Constructor that "moves" the data pointers.
     * This is essentially a convenience function that combines IrSequence and IrSignal constructors.
     *
     * @param intro
     * @param lengthIntro
     * @param repeat
     * @param lengthRepeat
     * @param ending
     * @param lengthEnding
     * @param frequency
     * @param dutyCycle
     */
    IrSignal(const microseconds_t *intro, size_t lengthIntro,
            const microseconds_t *repeat, size_t lengthRepeat,
            const microseconds_t *ending, size_t lengthEnding,
            frequency_t frequency = defaultFrequency,
            dutycycle_t dutyCycle = defaultDutyCycle);

    /**
     * Constructor that "moves" the data pointers. No ending sequence.
     * This is essentially a convenience function that combines IrSequence and IrSignal constructors.
     *
     * @param intro
     * @param lengthIntro
     * @param repeat
     * @param lengthRepeat
     * @param frequency
     * @param dutyCycle
     */
    IrSignal(const microseconds_t *intro, size_t lengthIntro,
            const microseconds_t *repeat, size_t lengthRepeat,
            frequency_t frequency = defaultFrequency,
            dutycycle_t dutyCycle = defaultDutyCycle);

    /**
     * Copy constructor.
     * @param intro
     * @param repeat
     * @param ending
     * @param frequency
     * @param dutyCycle
     */
    IrSignal(const IrSequence& intro, const IrSequence& repeat, const IrSequence& ending,
            frequency_t frequency = defaultFrequency, dutycycle_t dutyCycle = defaultDutyCycle);

    /**
     * Move constructor.
     *
     * @param intro
     * @param repeat
     * @param ending
     * @param frequency
     * @param dutyCycle
     */
    IrSignal(IrSequence&& intro, IrSequence&& repeat, IrSequence&& ending,
            frequency_t frequency = defaultFrequency, dutycycle_t dutyCycle = defaultDutyCycle);

    /**
     * Copy constructors for IrSignals without ending sequence.
     *
     * @param intro
     * @param repeat
     * @param frequency
     * @param dutyCycle
     */
    IrSignal(const IrSequence& intro, const IrSequence& repeat,
            frequency_t frequency = defaultFrequency, dutycycle_t dutyCycle = defaultDutyCycle);

    /**
     * Move constructor for IrSignals without ending sequence.
     * @param intro
     * @param repeat
     * @param frequency
     * @param dutyCycle
     */
    IrSignal(IrSequence&& intro, IrSequence&& repeat,
            frequency_t frequency = defaultFrequency, dutycycle_t dutyCycle = defaultDutyCycle);

    static IrSignal* readFlash(const microseconds_t *intro, size_t lengthIntro,
            const microseconds_t *repeat, size_t lengthRepeat,
            const microseconds_t *ending, size_t lengthEnding,
            frequency_t frequency = defaultFrequency,
            dutycycle_t dutyCycle = defaultDutyCycle);

    static IrSignal* readFlash(const microseconds_t *intro, size_t lengthIntro,
            const microseconds_t *repeat, size_t lengthRepeat,
            frequency_t frequency = defaultFrequency,
            dutycycle_t dutyCycle = defaultDutyCycle);

    frequency_t getFrequency() const {
        return frequency;
    }

    dutycycle_t getDutyCycle() const {
        return dutyCycle;
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
     * True if and only if at least one of the sequences is true (= non-empty).
     * @return
     */
    operator bool() const { return intro || repeat || ending; }

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
     * If the duty cycle is sensible, print it to the stream and return true.
     * Otherwise do nothing and return false.
     * No extra spaces or line feeds are generated.
     * @param stream Stream onto the output is printed.
     */
    bool dumpDutyCycle(Stream& stream) const {
        return dumpDutyCycle(stream, dutyCycle);
    }

    /**
     * Static version of dumpDutyCycle.
     * @param stream Stream onto the output is printed.
     * @param dutyCycle
     */
    static bool dumpDutyCycle(Stream& stream, dutycycle_t dutyCycle);

    /**
     * Implementation of the count semantics, i.e.,
     * how many repetitions should be sent if the signal is sent noSend times.
     * @param noSends number of times to "send signal".
     */
    unsigned int noRepetitions(unsigned int noSends) const {
        return noSends == 0 ? 0
                : intro ? noSends -1 : noSends;
    }
};
