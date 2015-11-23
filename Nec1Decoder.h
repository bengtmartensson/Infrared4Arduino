#ifndef NEC1DECODER_H
#define	NEC1DECODER_H

#include "IrDecoder.h"
#include "IrReader.h"

class Nec1Decoder : public IrDecoder {
private:
    static const microseconds_t timebase = 564;
    static const microseconds_t timebaseUpper = 650;
    static const microseconds_t timebaseLower = 450;

    // NOTE: use a signed type to be able to return the value invalid.
    int F;
    int D;
    int S;
    bool ditto;

    char decode[17];

    static boolean getDuration(microseconds_t duration, unsigned int time) {
        return duration <= time * timebaseUpper
                && duration >= time * timebaseLower;
    }
    static int decodeParameter(const IrReader &irCapturer, unsigned int index);
    static int decodeFlashGap(microseconds_t flash, microseconds_t gap);
public:
    Nec1Decoder();
    Nec1Decoder(const IrReader& irReader);
    virtual ~Nec1Decoder() {};

    int getF() const {
        return F;
    }
    int getD() const {
        return D;
    }
    int getS() const {
        return S;
    }
    boolean isDitto() const {
        return ditto;
    };

    static boolean tryDecode(const IrReader &irCapturer, Stream& string);

    const char *getDecode() const {
        return decode;
    };

};

#endif	/* NEC1DECODER_H */
