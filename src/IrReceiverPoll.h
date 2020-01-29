#pragma once

#include "IrReceiver.h"

/**
 * @class IrReceiverPoll
 * An implementation of IrReceiver using polling of the input pin.
 * It uses no timer or other hardware resources, and should thus run
 * on all platforms.
 */
class IrReceiverPoll : public IrReceiver {
private:
    /** Data buffer */
    microseconds_t *durationData;

    /** Number of valid entries in durationData */
    size_t dataLength;

public:
    IrReceiverPoll(size_t captureLength = defaultCaptureLength,
            pin_t pin = defaultPin,
            bool pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

    ~IrReceiverPoll();

    bool isReady() const {
        return timeouted || !isEmpty();
    }

    void reset();

    size_t getDataLength() const {
        return dataLength;
    }

    microseconds_t getDuration(unsigned int i) const {
        return durationData[i];
    }

    void setEndingTimeout(milliseconds_t timeOut) {
        endingTimeout = timeOut;
    }

    milliseconds_t getEndingTimeout() const {
        return endingTimeout;
    }

    milliseconds_t getBeginningTimeout() const {
        return beginningTimeout;
    }

    void setBeginningTimeout(milliseconds_t timeOut) {
        beginningTimeout = timeOut;
    }

    /**
     * In this class, enable does the actual collection of the data.
     * It returns when either the signal is captured, or (beginning)
     * timeout occurs.
     */
    void enable();

    void disable() {};

private:
    bool searchForStart();

    void collectData();

    void recordDuration(unsigned long t);
};
