#ifndef IRRECEIVERSAMPLE_H
#define	IRRECEIVERSAMPLE_H

#include "IrReceiver.h"

/** microseconds per clock interrupt */
#define USECPERTICK 50  // FIXME

/**
 * Singleton class due to ISP
 */
// The interrupt routine must have access to some stuff here.
// This needs to be public, and, since the interrupt routing
// fumbles with it, volatile.
class IrReceiverSampler : public IrReceiver {
public:
    /** State space for the receiver state machine. */
    enum ReceiverState_t {
        STATE_IDLE, /**< Between signals; waiting for first mark */
        STATE_MARK, /**< Last read a mark */
        STATE_SPACE, /**< Last read a space */
        STATE_STOP  /**< Complete signal has been read */
    };

    /** State of the state machine */
    volatile ReceiverState_t receiverState; // previously rcvstate;

    uint32_t endingTimeoutInTicks; // previously GAP_TICKS

    uint32_t beginningTimeoutInTicks; // previously TIMEOUT_TICKS;

    /** state timer, counts 50uS ticks. */
    volatile uint32_t timer;

    /** Data buffer */
    volatile microseconds_t *durationData; // previously rawbuf;

    /** Number of entries in durationData */
    volatile unsigned int dataLength; // previously rawlen

private:
    static IrReceiverSampler *instance;
    static uint32_t millisecs2ticks(milliseconds_t ms);
    static milliseconds_t ticks2millisecs(uint32_t tix);

protected:
    virtual ~IrReceiverSampler();

private:
    IrReceiverSampler(unsigned int captureLength = defaultCaptureLength,
            pin_t pin = defaultPin,
            boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

public:
    static IrReceiverSampler *newIrReceiverSampler(unsigned int captureLength = defaultCaptureLength,
            pin_t pin = defaultPin, boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

    static void deleteInstance();

    static IrReceiverSampler *getInstance() {
        return instance;
    }

    /** Enable the receiver for reception. */
    void enable();

    /** Disables IR reception. */
    void disable();

    /** Initializes the state machine; does not enable. */
    void reset();

    void setEndingTimeout(milliseconds_t timeOut);

    milliseconds_t getEndingTimeout() const;

    void setBeginningTimeout(milliseconds_t timeOut);

    milliseconds_t getBeginningTimeout() const;

    unsigned int getDataLength() const {
        return dataLength;
    }

    microseconds_t getDuration(unsigned int i) const {
        return USECPERTICK * durationData[i] + (i & 1 ? markExcess : -markExcess);
    }

    boolean isReady() const {
        return receiverState == STATE_STOP;
    }
};

#endif	/* IRRECEIVERSAMPLE_H */
