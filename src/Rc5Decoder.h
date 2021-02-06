#pragma once

#include "IrDecoder.h"
#include "IrReader.h"

/**
 * A decoder class for RC5 signals.
 */
class Rc5Decoder : public IrDecoder {
public:

    /**
     * Constructs a Rc5Decoder from an IrReader, containing data.
     * @param irReader IrReader with data, i.e. with isReady() true.
     */
    Rc5Decoder(const IrReader& irReader);

    virtual ~Rc5Decoder() {
    }

    /**
     * Returns the F parameter, or -1 if invalid.
     * @return int
     */
    int getF() const {
        return F;
    }

    /**
     * Returns the D parameter, or -1 if invalid.
     * @return int
     */
    int getD() const {
        return D;
    }

    /**
     * Returns the T parameter, or -1 if invalid.
     * @return int
     */
    int getT() const {
        return T;
    }

    /**
     * Convenience function; constructs an Rc5Decoder and calls its printDecode.
     * @param irReader IrReader to use
     * @param stream Stream
     * @return success of operation
     */
    static bool tryDecode(const IrReader& irReader, Stream& stream);

    const char *getDecode() const;

private:
    char decode[13];
    const static microseconds_t timebase = 889U;
    const static microseconds_t timebaseLower = 800U;
    const static microseconds_t timebaseUpper = 1000U;
    static bool getDuration(microseconds_t duration, unsigned int time) {
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

    static constexpr const char *format = "RC5 %d %d %d";

    static Length decodeDuration(microseconds_t t);
    static unsigned int decodeFlashGap(microseconds_t flash, microseconds_t gap);
};
