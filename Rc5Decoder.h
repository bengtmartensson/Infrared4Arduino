#ifndef RC5DECODER_H
#define	RC5DECODER_H

#include "IrDecoder.h"
#include "IrReader.h"

class Rc5Decoder : public IrDecoder {
public:
    Rc5Decoder(const IrReader& irReader);
    virtual ~Rc5Decoder() {}
    String toString() const;

    int getF() const {
        return F;
    }

    int getD() const {
        return D;
    }

    int getT() const {
        return T;
    }
    static boolean tryDecode(const IrReader& irReader, Stream& string);

private:
    const static microseconds_t timebase = 889U;
    const static microseconds_t timebaseLower = 800U;
    const static microseconds_t timebaseUpper = 1000U;
    static boolean getDuration(microseconds_t duration, unsigned int time) {
        return duration <= time * timebaseUpper
                && duration >= time * timebaseLower;
    }
    int F;
    int D;
    int T;

    enum Length {
        invalid = 0,
        half = 1,
        full = 2
    };

    static Length decode(microseconds_t t);
    static unsigned int decode(microseconds_t flash, microseconds_t gap);
};

#endif	/* RC5DECODER_H */
