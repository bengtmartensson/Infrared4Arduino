#pragma once

#include "IrReceiver.h"

/**
 * @class IrReceiverSampler
 * This receiving class samples the input pin every 50 microseconds using a timer
 * interrupt. Due to the ISP, this is a singleton class;
 * it can only be instantiated once.
 * This is enforced by the absence of public constructors:
 * it has to be instantiated by the
 * factory method newIrReceiverSampler.
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
            bool pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

public:
    /**
     * This factory method replaces public constructors. Provided that no instance currently exists,
     * it constructs a new instance and return a pointer to it, if possible. Otherwise, it returns nullptr.
     *
     * @param captureLength buffersize requested
     * @param pin GPIO pin to use
     * @param pullup true if the internal pullup resistor should be enabled
     * @param markExcess markExcess to use
     * @param beginningTimeout beginningTimeout to use
     * @param endingTimeout endingTimeout to use
     * @return pointer to a valid instance, or nullptr.
     */
    static IrReceiverSampler *newIrReceiverSampler(size_t captureLength = defaultCaptureLength,
            pin_t pin = defaultPin,
            bool pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

    /**
     * Deletes the instance, thereby freeing up the resources it occupied, and
     * allowing for another instance to be created.
     */
    static void deleteInstance();

    /**
     * Returns a pointer to the instance, or nullptr.
     * @return pointer to instance, possibly nullptr.
     */
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
        uint32_t bigvalue = Board::microsPerTick * static_cast<uint32_t>(durationData[i]) + (i & 1 ? markExcess : -markExcess);
        return bigvalue <=  MICROSECONDS_T_MAX ? (microseconds_t) bigvalue : MICROSECONDS_T_MAX;
    }

    bool isReady() const {
        return receiverState == STATE_STOP;
    }
};
