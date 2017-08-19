#include "IrReceiverSampler.h"
#include <IrTimerDefs.h>

uint32_t IrReceiverSampler::millisecs2ticks(milliseconds_t ms) {
    return (1000UL * (uint32_t) ms) / microsPerTick;
}

milliseconds_t IrReceiverSampler::ticks2millisecs(uint32_t tix) {
    return (milliseconds_t) ((tix * microsPerTick)/1000UL);
}

IrReceiverSampler *IrReceiverSampler::instance = NULL;

IrReceiverSampler::IrReceiverSampler(size_t captureLength,
        pin_t pin_,
        bool pullup,
        microseconds_t markExcess,
        milliseconds_t beginningTimeout,
        milliseconds_t endingTimeout) : IrReceiver(captureLength, pin_, pullup, markExcess) {
    setBeginningTimeout(beginningTimeout);
    setEndingTimeout(endingTimeout);
    durationData = new microseconds_t[bufferSize];
    dataLength = 0;
    timer = 0;
    receiverState = STATE_IDLE;
}

IrReceiverSampler *IrReceiverSampler::newIrReceiverSampler(size_t captureLength,
        pin_t pin,
        bool pullup,
        microseconds_t markExcess,
        milliseconds_t beginningTimeout,
        milliseconds_t endingTimeout) {
    if (instance != NULL || pin == invalidPin)
        return NULL;
    instance = new IrReceiverSampler(captureLength, pin, pullup, markExcess, beginningTimeout, endingTimeout);
    return instance;
}

void IrReceiverSampler::deleteInstance() {
    delete instance;
    instance = NULL;
}

IrReceiverSampler::~IrReceiverSampler() {
    delete [] durationData;
}

/*
 * The original IRrecv which uses 50us timer driven interrupts to sample input pin.
 */
void IrReceiverSampler::reset() {
    receiverState = STATE_IDLE;
    dataLength = 0;
}

void IrReceiverSampler::enable() {
    reset();
    noInterrupts();
    TIMER_CONFIG_NORMAL();
    TIMER_ENABLE_INTR;
    TIMER_RESET;
    interrupts();
}

void IrReceiverSampler::disable() {
    TIMER_DISABLE_INTR;
}

void IrReceiverSampler::setEndingTimeout(milliseconds_t timeOut) {
    endingTimeoutInTicks = millisecs2ticks(timeOut);
}

void IrReceiverSampler::setBeginningTimeout(milliseconds_t timeOut) {
    beginningTimeoutInTicks = millisecs2ticks(timeOut);
}

milliseconds_t IrReceiverSampler::getEndingTimeout() const {
    return ticks2millisecs(endingTimeoutInTicks);
}

milliseconds_t IrReceiverSampler::getBeginningTimeout() const {
    return ticks2millisecs(beginningTimeoutInTicks);
}

/** Interrupt routine. It collects data into the data buffer. */
ISR(TIMER_INTR_NAME) {
    IrReceiverSampler *recv = IrReceiverSampler::getInstance();
    IrReceiver::irdata_t irdata = recv->readIr();
    recv->timer++; // One more 50us tick
    if (recv->dataLength >= recv->getBufferSize()) {
        // Buffer full
        recv->receiverState = IrReceiverSampler::STATE_STOP;
    }
    switch (recv->receiverState) {
        case IrReceiverSampler::STATE_IDLE: // Looking for first mark
            if (irdata == IrReceiver::IR_MARK) {
                // Got the first mark, record duration and start recording transmission
                recv->dataLength = 0;
                recv->timer = 0;
                recv->receiverState = IrReceiverSampler::STATE_MARK;
            } else {
                if (recv->timer >= recv->beginningTimeoutInTicks) {
                    recv->durationData[recv->dataLength] = recv->timer;
                    recv->timer = 0;
                    recv->receiverState = IrReceiverSampler::STATE_STOP;
                }
            }
            break;
        case IrReceiverSampler::STATE_MARK:
            if (irdata == IrReceiver::IR_SPACE) {
                // MARK ended, record time
                recv->durationData[recv->dataLength++] = recv->timer;
                recv->timer = 0;
                recv->receiverState = IrReceiverSampler::STATE_SPACE;
            }
            break;
        case IrReceiverSampler::STATE_SPACE:
            if (irdata == IrReceiver::IR_MARK) {
                // SPACE just ended, record it
                recv->durationData[recv->dataLength++] = recv->timer;
                recv->timer = 0;
                recv->receiverState = IrReceiverSampler::STATE_MARK;
            } else {
                // still silence, is it over?
                if (recv->timer > recv->endingTimeoutInTicks) {
                    // big SPACE, indicates gap between codes
                    recv->durationData[recv->dataLength++] = recv->timer;
                    //recv->timer = 0;
                    recv->receiverState = IrReceiverSampler::STATE_STOP;
                }
            }
            break;
        case IrReceiverSampler::STATE_STOP:
            break;
        default:
            // should not happen
            break;
    }
}
