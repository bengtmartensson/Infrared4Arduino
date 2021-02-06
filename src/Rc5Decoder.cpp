#include "Rc5Decoder.h"
#include <string.h>

Rc5Decoder::Length Rc5Decoder::decodeDuration(microseconds_t t) {
    Length len =  (t < timebaseLower) ? invalid
            : (t <= timebaseUpper) ? half
            : (t >= 2*timebaseLower && t <= 2*timebaseUpper) ? full
            : invalid;
    return len;
}

unsigned int Rc5Decoder::decodeFlashGap(microseconds_t flash, microseconds_t gap) {
    bool result = getDuration(flash, 1);
    if (!result)
        return invalid;

    return getDuration(gap, 3) ? 1
            : getDuration(gap, 1) ? 0
            : invalid;
}

bool Rc5Decoder::tryDecode(const IrReader& irCapturer, Stream& stream) {
    Rc5Decoder decoder(irCapturer);
    return decoder.printDecode(stream);
}

Rc5Decoder::Rc5Decoder(const IrReader& irCapturer) {
    unsigned int index = 0U;
    unsigned int sum = 0U;
    int doublet = -1;
    decode[0] = '\0';

    while (doublet < 25) {
        Length length = decodeDuration(irCapturer.getDuration(index++));
        if (length == invalid)
            return;
        doublet += (int) length;
        if (doublet % 2 == 1)
            sum = (sum << 1U) + (index & 1U);
    }
    sum = ~sum & 0x1FFFU;

    bool success = isEnding(irCapturer.getDuration(irCapturer.getDataLength()-1));
    if (!success)
        return;

    F = (sum & 0x3FU) | ((~sum & 0x1000U) >> 6U);
    D = (sum & 0x7C0U) >> 6U;
    T = (sum & 0x0800U) >> 11U;

    setValid(true);
    sprintf(decode, format, D, F, T);
}

const char *Rc5Decoder::getDecode() const {
    return decode;
}
