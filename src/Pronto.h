/**
 * Static class consisting of tho functions for parsing a Pronto Hex string (like 0000 006C 0022 0002 015B 00AD ...)  into an IrSignal.
 * <a href="http://harctoolbox.org/Glossary.html#ProntoSemantics">Reference</a>.
 */

#ifndef PRONTO_H
#define	PRONTO_H

#include "InfraredTypes.h"
#include "IrSignal.h"

class Pronto {
private:
    typedef uint16_t prontoInt;

    static const prontoInt learnedToken = 0x0000;
    static const prontoInt learnedNonModulatedToken = 0x0100;
    static const unsigned int bitsInHexadecimal = 4;
    static const unsigned int digitsInProntoNumber = 4;
    static const unsigned int numbersInPreamble = 4;
    static const unsigned int hexMask = 0xF;
    static const unsigned int charsInPreamble = numbersInPreamble * (digitsInProntoNumber + 1);
    static constexpr double prontoFreqConst = 0.241246;
    static const uint32_t prontoConst = (uint32_t) (1E6 / prontoFreqConst); // 4145146
    static const prontoInt fallbackFrequencyCode = 0x0040; // To use with frequency = 0;
    static const frequency_t fallbackFrequency = 64767; // To use with frequency = 0;

    Pronto() {};

    static IrSequence *mkSequence(const uint16_t *data, size_t pairs, double timebase);

    static frequency_t toFrequency(prontoInt code);

public:
    /**
     * Function for parsing its input data into an IrSignal. The ending sequence will always be empty.
     * @param data Numerical data, the number in the Pronto form.
     * @param size Number of data points.
     * @return IrSignal
     */
    static IrSignal *parse(const uint16_t *data, size_t size);

    /**
     * Function for parsing its input data into an IrSignal. The ending sequence will always be empty.
     * @param str Text string containing a Pronto form signal.
     * @return IrSignal
     */
    static IrSignal *parse(const char *str);

    // TODO: static IrSignal *parse(const __FlashStringHelper *str);
};

#endif	/* PRONTO_H */

