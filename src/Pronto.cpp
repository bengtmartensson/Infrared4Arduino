#include "Pronto.h"
#include "IrSignal.h"
#include <string.h>

IrSignal *Pronto::parse(const uint16_t *data, size_t size) {
    double timebase = prontoFreqConst * data[1];
    frequency_t frequency;
    switch (data[0]) {
        case learnedToken: // normal, "learned"
             frequency = toFrequency(data[1]);
             break;
        case learnedNonModulatedToken: // non-demodulated, "learned"
            frequency = (frequency_t) 0;
            break;
        default:
            return NULL;
    }
    size_t introPairs = data[2];
    size_t repetitionPairs = data[3];
    if (numbersInPreamble + 2*(introPairs + repetitionPairs) != size) // inconsistent sizes
        return NULL;

    IrSequence *intro = mkSequence(data + numbersInPreamble, introPairs, timebase);
    IrSequence *repeat = mkSequence(data + numbersInPreamble + 2*introPairs, repetitionPairs, timebase);
    IrSequence *ending = new IrSequence();

    IrSignal *code = new IrSignal(*intro, *repeat, *ending, frequency, true);

    delete intro;
    delete repeat;
    delete ending;

    return code;
}

IrSignal *Pronto::parse(const char *str) {
    size_t len = strlen(str)/(digitsInProntoNumber + 1) + 1;
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
    return new IrSequence(durations, 2*noPairs, false);
}

frequency_t Pronto::toFrequency(prontoInt code) {
    return (frequency_t) (prontoConst / code);
}

Pronto::prontoInt Pronto::toFrequencyCode(frequency_t frequency) {
    return (prontoInt) (prontoConst / frequency);
}

size_t Pronto::lengthHexString(size_t introLength, size_t repeatLength) {
    return (digitsInProntoNumber + 1) * (numbersInPreamble + introLength + repeatLength);
}

char Pronto::hexDigit(unsigned int x) {
    return (char) (x <= 9 ? ('0' + x) : ('A' + (x - 10)));
}

unsigned int Pronto::appendChar(char *result, unsigned int index, char ch) {
    result[index] = ch;
    index++;
    return index;
}

unsigned int Pronto::appendDigit(char *result, unsigned int index, unsigned int number) {
    return appendChar(result, index, hexDigit(number)); //(number >> shifts) & 0xF);
}

unsigned int Pronto::appendNumber(char *result, unsigned int index, prontoInt number) {
    for (unsigned int i = 0; i < digitsInProntoNumber; i++) {
        unsigned int shifts = bitsInHexadecimal * (digitsInProntoNumber - 1 - i);
        index = appendDigit(result, index, (number >> shifts) & hexMask);
    }
    return appendChar(result, index, ' ');
}

unsigned int Pronto::appendDuration(char *result, unsigned int index, microseconds_t duration, frequency_t frequency) {
    return appendNumber(result, index, (unsigned int)(1E-6 * duration * frequency + 0.5));
}

unsigned int Pronto::appendSequence(char *result, unsigned int index, const microseconds_t *data, size_t length, frequency_t frequency) {
    frequency_t effectiveFrequency = frequency > 0 ? frequency : fallbackFrequency;
    for (unsigned int i = 0; i < length; i++)
        index = appendDuration(result, index, data[i], effectiveFrequency);
    return index;
}

unsigned int Pronto::appendSequence(char *result, unsigned int index, const IrSequence& irSequence, frequency_t frequency) {
    return appendSequence(result, index, irSequence.getDurations(), irSequence.getLength(), frequency);
}

char* Pronto::setup(frequency_t frequency, size_t introLength, size_t repeatLength) {
    char *result = new char[lengthHexString(introLength, repeatLength)];
    unsigned int index = 0;
    index = appendNumber(result, index, frequency > 0 ? learnedToken : learnedNonModulatedToken);
    index = appendNumber(result, index, toFrequencyCode(frequency));
    index = appendNumber(result, index, introLength / 2);
    index = appendNumber(result, index, repeatLength / 2);
    return result;
}

char* Pronto::toProntoHex(const microseconds_t* data, size_t length, frequency_t frequency) {
    char *result = setup(frequency, length, 0);
    unsigned int index = charsInPreamble;
    index = appendSequence(result, index, data, length, frequency);
    appendChar(result, index - 1, '\0');
    return result;
}

char* Pronto::toProntoHex(const IrSequence& irSequence, frequency_t frequency) {
    return toProntoHex(irSequence.getDurations(), irSequence.getLength(), frequency);
}

char* Pronto::toProntoHex(const IrSignal& irSignal) {
    char *result = setup(irSignal.getFrequency(), irSignal.getIntro().getLength(), irSignal.getRepeat().getLength());
    unsigned int index = charsInPreamble;
    index = appendSequence(result, index, irSignal.getIntro(), irSignal.getFrequency());
    index = appendSequence(result, index, irSignal.getRepeat(), irSignal.getFrequency());
    appendChar(result, index - 1, '\0');
    return result;
}
