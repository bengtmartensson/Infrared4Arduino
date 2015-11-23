#ifndef IRDECODER_H
#define	IRDECODER_H

#include "InfraredTypes.h"

class IrDecoder {
public:
    IrDecoder() {
        valid = false;
    }

    virtual ~IrDecoder() {}

    virtual const char *getDecode() const = 0;

    virtual boolean isValid() const {
        return valid;
    }

    boolean printDecode(Stream& stream) const {
        if (isValid())
            stream.println(getDecode());
        return isValid();
    }

private:
    const static uint32_t endingMin = 20000U;
    boolean valid;

protected:
    static const int invalid = -1;
    void setValid(bool valid_) {
        valid = valid_;
    }

    static boolean isEnding(microseconds_t duration) {
        return duration > endingMin;
    }
};
#endif	/* IRDECODER_H */
