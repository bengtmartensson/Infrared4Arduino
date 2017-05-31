#include "Pronto.h"
#include "IrSignal.h"
#include <string.h>

IrSignal *Pronto::parse(const uint16_t *data, size_t size) {
    double timebase = 0.241246 * data[1];
    frequency_t frequency;
    switch (data[0]) {
        case 0x0000: // normal, "learned"
             frequency = toFrequency(data[1]);
             break;
        case 0x0100: // non-demodulated, "learned"
            frequency = (frequency_t) 0;
            break;
        default:
            return NULL;
    }
    size_t introPairs = data[2];
    size_t repetitionPairs = data[3];
    if (4 + 2*(introPairs + repetitionPairs) != size) // inconsistent sizes
        return NULL;

    IrSequence *intro = mkSequence(data + 4, introPairs, timebase);
    IrSequence *repeat = mkSequence(data + 4 + 2*introPairs, repetitionPairs, timebase);
    IrSequence *ending = new IrSequence();

    IrSignal *code = new IrSignal(*intro, *repeat, *ending, frequency, true);

    delete intro;
    delete repeat;
    delete ending;

    return code;
}

IrSignal *Pronto::parse(const char *str) {
    size_t len = strlen(str)/5 + 1;
    uint16_t data[len];
    unsigned int index = 0;
    const char *p = str;
    char *endptr[1] = { NULL };
    while (*p) {
        long x = strtol(p, endptr, 16);
        data[index++] = (uint16_t) x; // If input is conforming, there can be no overflow!
        p = *endptr;
    }
    return parse(data, index);
}

IrSequence *Pronto::mkSequence(const uint16_t* data, size_t noPairs, double timebase) {
    microseconds_t *durations = new microseconds_t[2*noPairs];
    for (unsigned int i = 0; i < 2*noPairs; i++) {
        uint32_t duration = (uint32_t) (data[i] * timebase);
        durations[i] = (microseconds_t)((duration <= MICROSECONDS_T_MAX) ? duration : MICROSECONDS_T_MAX);
    }
    return new IrSequence(durations, 2*noPairs, true);
}
