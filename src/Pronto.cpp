#include "Pronto.h"
#include "IrSignal.h"
#include "Board.h" // for HAS_FLASH_READ
#include <string.h>

inline uint16_t divRound(microseconds_t duration, microseconds_t timebase) {
    return static_cast<uint16_t>((static_cast<uint32_t>(duration) + timebase / 2) / timebase);
}

IrSignal *Pronto::parse(const uint16_t *data, size_t size) {
    microseconds_t timebase = (microsecondsInSeconds * data[1] + referenceFrequency/2) / referenceFrequency;
    frequency_t frequency;
    switch (data[0]) {
        case learnedToken: // normal, "learned"
             frequency = toFrequency(data[1]);
             break;
        case learnedNonModulatedToken: // non-demodulated, "learned"
            frequency = 0U;
            break;
        default:
            return nullptr;
    }
    size_t introPairs = data[2];
    size_t repetitionPairs = data[3];
    if (numbersInPreamble + 2*(introPairs + repetitionPairs) != size) // inconsistent sizes
        return nullptr;

    microseconds_t* intro = mkSequence(data + numbersInPreamble, introPairs, timebase);
    microseconds_t* repeat = mkSequence(data + numbersInPreamble + 2*introPairs, repetitionPairs, timebase);

    return new IrSignal(intro, 2*introPairs, repeat, 2*repetitionPairs, frequency);
}

IrSignal *Pronto::parse(const char *str) {
    size_t len = strlen(str)/(digitsInProntoNumber + 1) + 1;
    uint16_t* data = new uint16_t[len];
    const char *p = str;
    char *endptr[1];
    for (unsigned int i = 0; i < len; i++) {
        long x = strtol(p, endptr, 16);
        if (x == 0 && i >= numbersInPreamble) {
            // Alignment error?, bail immediately (often right result).
            len = i;
            break;
        }
        data[i] = x; // If input is conforming, there can be no overflow!
        p = *endptr;
    }
    IrSignal *res = parse(data, len);
    delete [] data;
    return res;
}

#if HAS_FLASH_READ || defined(DOXYGEN)
IrSignal *Pronto::parse_PF(uint_farptr_t str) {
    size_t len = strlen_PF(STRCPY_PF_CAST(str));
    char work[len + 1];
    strncpy_PF(work, STRCPY_PF_CAST(str), len);
    return parse(work);
}

IrSignal *Pronto::parse_PF(const char *str) {
    return parse_PF(reinterpret_cast<uint_farptr_t>(str)); // to avoid infinite recursion
};

IrSignal *Pronto::parse(const __FlashStringHelper *str) {
    return parse_PF(reinterpret_cast<uint_farptr_t>(str));
}
#endif

microseconds_t* Pronto::mkSequence(const uint16_t* data, size_t noPairs, microseconds_t timebase) {
    microseconds_t *durations = new microseconds_t[2*noPairs];
    for (unsigned int i = 0; i < 2*noPairs; i++) {
        uint32_t duration = static_cast<uint32_t>(data[i]) * timebase;
        durations[i] = (duration <= MICROSECONDS_T_MAX) ? duration : MICROSECONDS_T_MAX;
    }
    return durations;
}

frequency_t Pronto::toFrequency(uint16_t code) {
    return referenceFrequency / code;
}

uint16_t Pronto::toFrequencyCode(frequency_t frequency) {
    return referenceFrequency / effectiveFrequency(frequency);
}

frequency_t Pronto::effectiveFrequency(frequency_t frequency) {
    return frequency > 0 ? frequency : fallbackFrequency;
}

microseconds_t Pronto::toTimebase(frequency_t frequency) {
    return microsecondsInSeconds / effectiveFrequency(frequency);
}

size_t Pronto::lengthHexString(size_t introLength, size_t repeatLength) {
    return (digitsInProntoNumber + 1) * (numbersInPreamble + introLength + repeatLength);
}

char Pronto::hexDigit(unsigned int x) {
    return x <= 9 ? ('0' + x) : ('A' + (x - 10));
}

void Pronto::appendChar(char *result, unsigned int& index, char ch) {
    result[index] = ch;
    index++;
}

void Pronto::appendDigit(char *result, unsigned int& index, unsigned int number) {
    appendChar(result, index, hexDigit(number));
}

void Pronto::appendNumber(char *result, unsigned int& index, uint16_t number) {
    for (unsigned int i = 0; i < digitsInProntoNumber; i++) {
        unsigned int shifts = bitsInHexadecimal * (digitsInProntoNumber - 1 - i);
        appendDigit(result, index, (number >> shifts) & hexMask);
    }
    appendChar(result, index, ' ');
}

void Pronto::appendDuration(char *result, unsigned int& index, microseconds_t duration, microseconds_t timebase) {
    appendNumber(result, index, divRound(duration, timebase));
}

void Pronto::appendSequence(char *result, unsigned int& index, const microseconds_t *data, size_t length, microseconds_t timebase) {
    for (unsigned int i = 0; i < length; i++)
        appendDuration(result, index, data[i], timebase);
}

void Pronto::appendSequence(char *result, unsigned int& index, const IrSequence& irSequence, microseconds_t timebase) {
    appendSequence(result, index, irSequence.getDurations(), irSequence.getLength(), timebase);
}

char* Pronto::prelude(frequency_t frequency, size_t introLength, size_t repeatLength) {
    char *result = new char[lengthHexString(introLength, repeatLength)];
    unsigned int index = 0;
    appendNumber(result, index, frequency > 0 ? learnedToken : learnedNonModulatedToken);
    appendNumber(result, index, toFrequencyCode(frequency));
    appendNumber(result, index, introLength / 2);
    appendNumber(result, index, repeatLength / 2);
    return result;
}

char* Pronto::toProntoHex(const microseconds_t* introData, size_t introLength, const microseconds_t* repeatData, size_t repeatLength, frequency_t frequency) {
    char *result = prelude(frequency, introLength, repeatLength);
    unsigned int index = numbersInPreamble * (digitsInProntoNumber + 1);
    microseconds_t timebase = toTimebase(frequency);
    appendSequence(result, index, introData, introLength, timebase);
    appendSequence(result, index, repeatData, repeatLength, timebase);
    result[index - 1] = '\0';
    return result;
}

void Pronto::dump(Stream& stream, const microseconds_t* introData, size_t introLength, const microseconds_t* repeatData, size_t repeatLength, frequency_t frequency) {
    dumpNumber(stream, frequency > 0 ? learnedToken : learnedNonModulatedToken);
    stream.print(' ');
    dumpNumber(stream, toFrequencyCode(frequency));
    stream.print(' ');
    dumpNumber(stream, introLength / 2);
    stream.print(' ');
    dumpNumber(stream, repeatLength / 2);
    stream.print(' ');
    microseconds_t timebase = toTimebase(frequency);
    dumpSequence(stream, introData, introLength, timebase);
    stream.print(' ');
    dumpSequence(stream, repeatData, repeatLength, timebase);
}

void Pronto::dumpSequence(Stream& stream, const microseconds_t *data, size_t length, microseconds_t timebase) {
    for (unsigned int i = 0; i < length; i++) {
        dumpDuration(stream, data[i], timebase);
        if (i < length - 1)
            stream.print(' ');
    }
}

void Pronto::dumpDuration(Stream& stream, microseconds_t duration, microseconds_t timebase) {
    dumpNumber(stream, divRound(duration, timebase));
}

void Pronto::dumpNumber(Stream& stream, uint16_t number) {
    for (unsigned int i = 0; i < digitsInProntoNumber; i++) {
        unsigned int shifts = bitsInHexadecimal * (digitsInProntoNumber - 1 - i);
        dumpDigit(stream, (number >> shifts) & hexMask);
    }
}

void Pronto::dumpDigit(Stream& stream, unsigned int number) {
    stream.print(hexDigit(number));
}
