#ifndef RECS80DECODER_H
#define	RECS80DECODER_H

#include "IrDecoder.h"
#include "IrReader.h"

/**
 * A decoder class for Recs80 signals.
 */
class Recs80Decoder : public IrDecoder {
private:
    static const microseconds_t timebase = 158;
    static const microseconds_t timebaseUpper = timebase * 1.4;
    static const microseconds_t timebaseLower = timebase * 0.7;

    // NOTE: use a signed type to be able to return the value invalid.
    int8_t F;
    int8_t D;
    int8_t T;

    char decode[14];

    static bool getDuration(microseconds_t duration, unsigned int time) {
        return duration <= time * timebaseUpper
                && duration >= time * timebaseLower;
    }
    static int decodeParameter(unsigned int length, const IrReader &irCapturer, unsigned int &index);
    static int decodeFlashGap(microseconds_t flash, microseconds_t gap);

public:
    Recs80Decoder();

    /**
     * Constructs a Recs80Decoder from an IrReader, containing data.
     * @param irReader IrReader with data, i.e. with isReady() true.
     */
    Recs80Decoder(const IrReader& irReader);
    virtual ~Recs80Decoder() {};

    /**
     * Returns the F parameter, or -1 if invalid.
     * @return int
     */
    int8_t getF() const {
        return F;
    }

    /**
     * Returns the D parameter, or -1 if invalid.
     * @return int
     */
    int8_t getD() const {
        return D;
    }

    /**
     * Returns the T parameter, or -1 if invalid.
     * @return int
     */
    int8_t getT() const {
        return T;
    }

    /**
     * Convenience function; constructs an Recs80Decoder and calls its printDecode.
     * @param irReader IrReader to use
     * @param stream Stream
     * @return success of operation
     */
    static bool tryDecode(const IrReader &irReader, Stream& stream);

    const char *getDecode() const {
        return decode;
    };

};

#endif	/* RECS80DECODER_H */
