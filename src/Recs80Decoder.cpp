#include "Recs80Decoder.h"
#include "IrReader.h"
#include <string.h>
#include <stdlib.h>

// {38k,158,msb}<1,-31|1,-47>(1:1,T:1,D:3,F:6,1,-45m)+ {}[D:0..7,F:0..63, T@:0..1=0]

int Recs80Decoder::decodeFlashGap(microseconds_t flash, microseconds_t gap) {
    bool result = getDuration(flash, 1);
    if (!result)
        return invalid;

    return getDuration(gap, 47) ? 1
            : getDuration(gap, 31) ? 0
            : invalid;
}

bool Recs80Decoder::tryDecode(const IrReader& irReader, Stream& stream) {
    Recs80Decoder decoder(irReader);
    return decoder.printDecode(stream);
}

int Recs80Decoder::decodeParameter(unsigned int length, const IrReader& irReader, unsigned int &index) {
    unsigned int sum = 0;
    for (unsigned int i = 0; i < length; i++) {
        int result = decodeFlashGap(irReader.getDuration(2 * i + index), irReader.getDuration(2 * i + 1 + index));
        if (result == invalid)
            return invalid;
        sum = (sum << 1) + result;
    }
    index += 2*length;
    return sum;
}

Recs80Decoder::Recs80Decoder(const IrReader &irReader) : IrDecoder() {
    decode[0] = '\0';
    unsigned int index = 0;
    bool success = false;
    if (irReader.getDataLength() != 24U)
        return;
    int8_t one = decodeParameter(1U, irReader, index);
    if (one != 1)
        return;

    T = decodeParameter(1U, irReader, index);
    if (T == invalid)
        return;

    D = decodeParameter(3U, irReader, index);
    if (D == invalid)
        return;

    F = decodeParameter(6U, irReader, index);
    if (F == invalid)
        return;

    success = getDuration(irReader.getDuration(index++), 1U);
    if (!success)
        return;

    setValid(true);
    sprintf(decode, "RECS80 %d %d %d", D, F, T);
}
