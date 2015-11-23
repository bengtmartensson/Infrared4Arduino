#ifndef _MULTIDECODER_H
#define _MULTIDECODER_H

#include "IrReader.h"
#include "IrDecoder.h"

class MultiDecoder : public IrDecoder {
public:
    enum Type {
        timeout,
        noise,
        undecoded,
        nec,
        nec_ditto,
        rc5
    };

private:
    char decode[17];
    Type type;

public:
    Type getType() const {
        return type;
    }

    MultiDecoder(const IrReader &irReader);

    virtual ~MultiDecoder() {
    }

    const char *getDecode() const {
        return decode;
    }
};

#endif // ! _MULTIDECODER_H
