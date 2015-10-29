#include "Nec1Decoder.h"
#include "IrReader.h"
//#include "IrReceiver.h"

//Nec1Decoder::~Nec1Decoder() {
//}
//{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,1,^108m,(16,-4,1,^108m)*) [D:0..255,S:0..255=255-D,F:0..255]

int Nec1Decoder::decodeFlashGap(microseconds_t flash, microseconds_t gap) {
    boolean result = getDuration(flash, 1);
    if (!result)
        return invalid;

    return getDuration(gap, 3) ? 1
            : getDuration(gap, 1) ? 0
            : invalid;
}

boolean Nec1Decoder::tryDecode(const IrReader& irReader, Stream& stream) {
    Nec1Decoder decoder(irReader);
    return decoder.printDecode(stream);
}

int Nec1Decoder::decodeParameter(const IrReader& irReader, unsigned int index) {
    unsigned int sum = 0;
    for (int i = 7; i >= 0; i--) {
        int result = decodeFlashGap(irReader.getDuration(2 * i + index), irReader.getDuration(2 * i + 1 + index));
        if (result == invalid)
            return invalid;
        sum = (sum << 1) + result;
    }
    return sum;
}

Nec1Decoder::Nec1Decoder(const IrReader &irReader) : IrDecoder() {
    unsigned int index = 0;
    boolean success;
    if (irReader.getDataLength() == 4U) {
        success = getDuration(irReader.getDuration(index++), 16U);
        if (!success)
            return;
        success = getDuration(irReader.getDuration(index++), 4U);
        if (!success)
            return;
        success = getDuration(irReader.getDuration(index++), 1U);
        if (!success)
            return;
        success = getEnding(irReader.getDuration(index));
        if (!success)
            return;
        ditto = true;
        setValid(true);
        decode = F("NEC1 ditto");
    } else if (irReader.getDataLength() == 34U * 2U) {
        success = getDuration(irReader.getDuration(index++), 16U);
        if (!success)
            return;
        success = getDuration(irReader.getDuration(index++), 8U);
        if (!success)
            return;
        D = decodeParameter(irReader, index);
        if (D == invalid)
            return;
        index += 16;
        S = decodeParameter(irReader, index);
        if (S == invalid)
            return;
        index += 16;
        F = decodeParameter(irReader, index);
        if (F == invalid)
            return;
        index += 16;
        int invF = decodeParameter(irReader, index);
        if (invF < 0)
            return;
        if ((F ^ invF) != 0xFF)
            return;
        index += 16;

        success = getDuration(irReader.getDuration(index++), 1U);
        if (!success)
            return;
        success = getEnding(irReader.getDuration(index));
        if (!success)
            return;
        ditto = false;
        setValid(true);
        decode = String(F("NEC1 ")) +
#ifdef ARDUINO
            String(D) + String(F(" ")) + String(S) + String(F(" ")) + String(F);
#else
            std::to_string(D) + " " + std::to_string(S) + " " + std::to_string(F);
#endif
    }
}
