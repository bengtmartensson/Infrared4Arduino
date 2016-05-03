#ifndef _MULTIDECODER_H
#define _MULTIDECODER_H

#include "IrReader.h"
#include "IrDecoder.h"

/**
 * A preliminary multi protocol decoder. Tries the Nec1- and the Rc5 decoders.
 */
class MultiDecoder : public IrDecoder {
public:
    /**
     * Enum over possible outcomes of the decoder.
     */
    enum Type {
        timeout,        ///< beginTimeout reached
        noise,          ///< nothing sensible found
        undecoded,      ///< decoding failed
        nec,            ///< NEC1 intro
        nec_ditto,      ///< NEC1 repeat
        rc5             ///< RC5 signal (= repeat sequence)
    };

private:
    char decode[17];
    Type type;

public:
    Type getType() const {
        return type;
    }

    /**
     * Constructs a MultiDecoder from an IrReader, containing data.
     * @param irReader IrReader with data, i.e. with isReady() true.
     */
    MultiDecoder(const IrReader &irReader);

    virtual ~MultiDecoder() {
    }

    const char *getDecode() const {
        return decode;
    }
};

#endif // ! _MULTIDECODER_H
