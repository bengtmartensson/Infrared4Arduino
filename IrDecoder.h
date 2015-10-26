#ifndef IRDECODER_H
#define	IRDECODER_H

#include "InfraredTypes.h"

class IrDecoder {
public:
    IrDecoder() {
        valid = false;
    }

    virtual ~IrDecoder() {}

    virtual String toString() const = 0;

    virtual boolean isValid() const {
        return valid;
    }

#ifdef ARDUINO
    boolean printDecode(Stream& stream) const {
        if (isValid())
            stream.println(toString());
        return isValid();
    }
#endif
    
private:
    //const static double tolerance = 0.1; //10% relative tolerance
    const static uint32_t endingMin = 20000U;

    boolean valid;

protected:
    static const int invalid = -1;
    void setValid(bool valid_) {
        valid = valid_;
    }

    static boolean getEnding(microseconds_t duration) {
        return duration > endingMin;
    }
};
#endif	/* IRDECODER_H */
