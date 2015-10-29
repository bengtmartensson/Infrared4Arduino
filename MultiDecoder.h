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
    Type type;

public:
    Type getType() const {
        return type;
    }

    MultiDecoder(const IrReader &irReader);

    virtual ~MultiDecoder() {
    }
};

#endif // ! _MULTIDECODER_H
