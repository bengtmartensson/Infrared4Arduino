#ifndef IRDECODER_H
#define	IRDECODER_H

#include "InfraredTypes.h"

class IrDecoder {
public:
    IrDecoder() {
        decode = "";
        valid = false;
    }

    virtual ~IrDecoder() {}

    const String& toString() const {
        return decode;
    }

    virtual boolean isValid() const {
        return valid;
    }

    boolean printDecode(Stream& stream) const {
        if (isValid())
            stream.println(toString());
        return isValid();
    }

private:
    const static uint32_t endingMin = 20000U;
    boolean valid;

protected:
    String decode;
    static const int invalid = -1;
    void setValid(bool valid_) {
        valid = valid_;
    }

    static boolean getEnding(microseconds_t duration) {
        return duration > endingMin;
    }
};
#endif	/* IRDECODER_H */
