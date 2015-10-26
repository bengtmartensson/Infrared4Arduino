#include "MultiDecoder.h"
#include "Nec1Decoder.h"
#include "Rc5Decoder.h"

MultiDecoder::MultiDecoder(const IrReader &IrReader) {
    if (IrReader.isEmpty()) {
        type = timeout;
        decode = F(".");
        return;
    }

    if (IrReader.getDataLength() < 3) {
        type = noise;
        decode = F(":");
        return;
    }

    Nec1Decoder nec1decoder(IrReader);
    if (nec1decoder.isValid()) {
        decode = nec1decoder.toString();
        type = nec1decoder.isDitto() ? nec_ditto : nec;
        setValid(true);
        return;
    }

    Rc5Decoder rc5decoder(IrReader);
    if (rc5decoder.isValid()) {
        decode = rc5decoder.toString();
        type = rc5;
        setValid(true);
        return;
    }

    // Giving up
    decode = String(F("*** ")) +
#ifdef ARDUINO
            String(IrReader.getDataLength())
#else
            std::to_string(IrReader.getDataLength())
#endif
            ;
    type = undecoded;
}
