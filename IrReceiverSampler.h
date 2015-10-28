#ifndef IRRECEIVERSAMPLE_H
#define	IRRECEIVERSAMPLE_H

#include "IrReceiver.h"

/**
 * Singleton class due to ISP
 */
// The interrupt routine must have access to some stuff here.
// This needs to be public, and, since the interrupt routing
// fumbles with it, volatile.
class IrReceiverSampler : public IrReceiver {
public:

    /** microseconds per clock interrupt */
    static const unsigned long microsPerTick = 50; // was USECPERTICK
    // Note doubled in IRLibTimer.h

    /** State space for the receiver state machine. */
    enum ReceiverState_t {
        STATE_IDLE, /**< Between signals; waiting for first mark */
        STATE_MARK, /**< Last read a mark */
        STATE_SPACE, /**< Last read a space */
        STATE_STOP  /**< Complete signal has been read */
    };

    /** State of the state machine */
    volatile ReceiverState_t receiverState; // previously rcvstate;

    // Note: use the following ones instead of beginningTimeout and endingTimeout
    // inherited from IrReader.
    uint32_t endingTimeoutInTicks; // previously GAP_TICKS

    uint32_t beginningTimeoutInTicks; // previously TIMEOUT_TICKS;

    /** state timer, counts 50uS ticks. */
    volatile uint32_t timer;

    /** Data buffer */
    volatile microseconds_t *durationData; // previously rawbuf;

    /** Number of entries in durationData */
    volatile size_t dataLength; // previously rawlen

private:
    static IrReceiverSampler *instance;
    static uint32_t millisecs2ticks(milliseconds_t ms);
    static milliseconds_t ticks2millisecs(uint32_t tix);

protected:
    virtual ~IrReceiverSampler();

private:
    IrReceiverSampler(size_t captureLength = defaultCaptureLength,
            pin_t pin = defaultPin,
            boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

public:
    static IrReceiverSampler *newIrReceiverSampler(size_t captureLength = defaultCaptureLength,
            pin_t pin = defaultPin,
            boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

    static void deleteInstance();

    static IrReceiverSampler *getInstance() {
        return instance;
    }

    void enable();

    void disable();

    void reset();

    void setEndingTimeout(milliseconds_t timeOut);

    milliseconds_t getEndingTimeout() const;

    void setBeginningTimeout(milliseconds_t timeOut);

    milliseconds_t getBeginningTimeout() const;

    size_t getDataLength() const {
        return dataLength;
    }

    microseconds_t getDuration(unsigned int i) const {
        return (microseconds_t) (microsPerTick * durationData[i] + (i & 1 ? markExcess : -markExcess));
    }

    boolean isReady() const {
        return receiverState == STATE_STOP;
    }
};

#endif	/* IRRECEIVERSAMPLE_H */
