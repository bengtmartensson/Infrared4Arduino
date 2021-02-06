#include "HashDecoder.h"

uint32_t HashDecoder::compare(microseconds_t oldVal, microseconds_t newVal) {
    return
    newVal < 3 * (oldVal / 4) ? 0
            : newVal > 5 * (oldVal / 4) ? 2
            : 1;
}

bool HashDecoder::tryDecode(const IrReader& irCapturer, Stream& stream) {
    HashDecoder decoder(irCapturer);
    return decoder.printDecode(stream);
}

void HashDecoder::decode(const microseconds_t* data, size_t length) {
    if (length < minMeaningfulLength)
        return;

    for (unsigned int i = 0; i < length - offset - 1; i++) {
        uint32_t value = compare(data[i], data[i + offset]);
        hash = (hash * FNVprime) ^ value;
    }

    setValid(true);
}

void HashDecoder::decode(const IrReader& irReader) {
    size_t length = irReader.getDataLength();
    if (length < minMeaningfulLength)
        return;

    for (unsigned int i = 0; i < length - offset - 1; i++) {
        uint32_t value = compare(irReader.getDuration(i), irReader.getDuration(i + offset));
        hash = (hash * FNVprime) ^ value;
    }

    setValid(true);
}

uint32_t HashDecoder::decodeHash(const IrSequence& irSequence) {
    HashDecoder decoder(irSequence);
    return decoder.getHash();
}

uint32_t HashDecoder::decodeHash(const IrReader& irReader) {
    HashDecoder decoder(irReader);
    return decoder.getHash();
}