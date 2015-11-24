/**
 * Parse a Pronto Hex string into an IrSignal.
 * Ref: http://harctoolbox.org/Glossary.html#ProntoSemantics
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
    static IrSignal *parse(const uint16_t *data, size_t size);

    static IrSignal *parse(const char *str);
};

#endif	/* PRONTO_H */

