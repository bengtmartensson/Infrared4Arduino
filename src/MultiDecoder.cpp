#include "MultiDecoder.h"
#include "Nec1Decoder.h"
#include "Rc5Decoder.h"
#include <string.h>

MultiDecoder::MultiDecoder(const IrReader &irReader) {
    if (! irReader) {
        type = timeout;
        strcpy(decode, ".");
        return;
    }

    if (irReader.getDataLength() < 3) {
        type = noise;
        strcpy(decode, ":");
        return;
    }

    Nec1Decoder nec1decoder(irReader);
    if (nec1decoder.isValid()) {
        strcpy(decode, nec1decoder.getDecode());
        type = nec1decoder.isDitto() ? nec_ditto : nec;
        setValid(true);
        return;
    }

    Rc5Decoder rc5decoder(irReader);
    if (rc5decoder.isValid()) {
        strcpy(decode, rc5decoder.getDecode());
        type = rc5;
        setValid(true);
        return;
    }

    // Giving up
    strcpy(decode, "***");
    type = undecoded;
}
