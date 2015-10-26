#include "Nec1Decoder.h"
#include "IrReader.h"
//#include "IrReceiver.h"

//Nec1Decoder::~Nec1Decoder() {
//}
//{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,1,^108m,(16,-4,1,^108m)*) [D:0..255,S:0..255=255-D,F:0..255]

String Nec1Decoder::toString() const {
    return !isValid() ? String()
            : ditto ? String(F("NEC1 ditto"))
            : String(F("NEC1 ")) +
#ifdef ARDUINO
            String(D) + String(F(" ")) + String(S) + String(F(" ")) + String(F);
#else
            std::to_string(D) + " " + std::to_string(S) + " " + std::to_string(F);
#endif
}

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

int Nec1Decoder::decode(const IrReader& irReader, unsigned int index) {
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
    } else if (irReader.getDataLength() == 34U * 2U) {
        success = getDuration(irReader.getDuration(index++), 16U);
        if (!success)
            return;
        success = getDuration(irReader.getDuration(index++), 8U);
        if (!success)
            return;
        D = decode(irReader, index);
        if (D == invalid)
            return;
        index += 16;
        S = decode(irReader, index);
        if (S == invalid)
            return;
        index += 16;
        F = decode(irReader, index);
        if (F == invalid)
            return;
        index += 16;
        int invF = decode(irReader, index);
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
    }
}

/*public static void main(String[] args) {
    try {
        IrSignal irSignal = new IrSignal("0000 006C 0022 0002 015B 00AD 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 06A4 015B 0057 0016 0E6C");
        IrSequence irSequence = irSignal.toModulatedIrSequence(3).noisify(20);
        IrSequence[] seqs = irSequence.chop(20000);
        for (IrSequence irs : seqs) {
            System.out.println(irs);
            Nec1Decoder decoder = new Nec1Decoder(irs);
            System.out.println(decoder);
        }

    } catch (IrpMasterException ex) {
        System.err.println(ex);
    }
}*/

