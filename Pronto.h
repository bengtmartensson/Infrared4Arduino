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
    Pronto() {};

    static IrSequence *mkSequence(const uint16_t *data, size_t pairs, double timebase);

    static frequency_t toFrequency(uint16_t code) {
        return (frequency_t) (4145146L/code);
    }

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
};

#endif	/* PRONTO_H */

