#ifndef IRDECODER_H
#define	IRDECODER_H

#include "InfraredTypes.h"

/**
 * Abstract base class for all decoder classes.
 */
class IrDecoder {
public:
    IrDecoder() {
        valid = false;
    }

    virtual ~IrDecoder() {}

    /**
     * Returns a textual description the decode for human consumption.
     * @return decode as text.
     */
    virtual const char *getDecode() const = 0;

    /**
     * Returns true if the decode was successful.
     * @return validity
     */
    virtual bool isValid() const {
        return valid;
    }

    /**
     * If valid, prints the decode to the stream.
     * @param stream where the output is generated
     * @return success status
     */
    bool printDecode(Stream& stream) const {
        if (isValid())
            stream.println(getDecode());
        return isValid();
    }

private:
    const static uint32_t endingMin = 20000U;
    bool valid;

protected:
    static const int invalid = -1;
    void setValid(bool valid_) {
        valid = valid_;
    }

    /**
     * Tests if the argument is large enough to be considered an ending of a decodable signal.
     * @param duration time to be tested
     * @return true if the duration is long enough
     */
    static bool isEnding(microseconds_t duration) {
        return duration > endingMin;
    }
};
#endif	/* IRDECODER_H */
