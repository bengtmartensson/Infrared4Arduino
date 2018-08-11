#include <Arduino.h>
#include "Tokenizer.h"

Tokenizer::Tokenizer(const String& str) : index(0), payload(str) {
    trim();
}

void Tokenizer::trim() {
};

Tokenizer::~Tokenizer() {
}

String Tokenizer::getRest() {
    String result = index == invalidIndex ? String("") :
            payload.substring(index);
    index = invalidIndex;
    return result;
}

String Tokenizer::getLine() {
    if (index == invalidIndex)
        return String("");

    int i = payload.indexOf('\n', index);
    String s = (i > 0) ?
            payload.substring(index, i) : payload.substring(index);
    index = (i > 0) ? i + 1 : invalidIndex;
    return s;
}

String Tokenizer::getToken() {
    if (index < 0)
        return String("");

    int i = payload.indexOf(' ', index);
    String s = (i > 0) ?
            payload.substring(index, i) : payload.substring(index);
    index = (i > 0) ? i : invalidIndex;
    if (index != invalidIndex)
        while (payload.charAt(index) == ' ')
            index++;
    return s;
}

long Tokenizer::getInt() {
    String token = getToken();
    return token == "" ? (long) invalid : token.toInt();
}

microseconds_t Tokenizer::getMicroseconds() {
    long t = getToken().toInt();
    return (microseconds_t) ((t < MICROSECONDS_T_MAX) ? t : MICROSECONDS_T_MAX);
}

frequency_t Tokenizer::getFrequency() {
    long t = getToken().toInt();
    return (frequency_t) ((t < FREQUENCY_T_MAX) ? t : FREQUENCY_T_MAX);
}
