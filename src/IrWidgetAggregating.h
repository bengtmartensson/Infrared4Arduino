// Copyright (c) 2012 Michael Dreher  <michael(at)5dot1.de>
// this code may be distributed under the terms of the General Public License V2 (GPL V2)

// This is a slight reorganization of the original code, by Bengt Martensson.

#ifndef IRWIDGETAGGREGATING_H
#define	IRWIDGETAGGREGATING_H

#include "IrWidget.h"

/**
 * This class implements the IrWidget. It delivers the duration and an estimate of the
 * modulation frequency based upon the collected samples. Since it uses a single timer,
 * it is singleton class, only instantiable by the factory method newIrWidgetAggregating.
 */
class IrWidgetAggregating : public IrWidget {
private:
    static IrWidgetAggregating *instance;
    IrWidgetAggregating() {
    }

public:
    void capture();
    static void deleteInstance();

    static IrWidgetAggregating *getInstance() {
        return instance;
    }

    static IrWidgetAggregating *newIrWidgetAggregating(size_t captureLength = defaultCaptureLength,
            bool pullup = false,
            int16_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

    microseconds_t inline getDuration(unsigned int i) const {
        uint32_t result32 = timerValueToNanoSeconds(unpackTimeVal(captureData[i])) / 1000
                + (i & 1 ? markExcess : -markExcess);
        return result32 <= MICROSECONDS_T_MAX ? (microseconds_t) result32 : MICROSECONDS_T_MAX;
    }

protected:
    IrWidgetAggregating(size_t captureLength = defaultCaptureLength,
            bool pullup = false,
            int16_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

private:
#ifdef ARDUINO
    bool waitForFirstEdge() const;

    void inline setupTriggerAndTimers();

    bool inline overflowBit(uint8_t tifr) const {
        return tifr & _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC));
    }

    uint8_t inline waitForTimer() {
        debugPinSet();
        uint8_t tifr;
        do {
            tifr = CAT2(TIFR, CAP_TIM) & (_BV(CAT2(ICF, CAP_TIM)) | _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC)));
        } while (!tifr);
        debugPinClear();
        return tifr;
    }

    // clear input capture and output compare flag bit
    void clearInputCaptureOutputCompareFlag() const {
        CAT2(TIFR, CAP_TIM) = _BV(CAT2(ICF, CAP_TIM)) | _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC));
    }

    void stopTimer() const {
        TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00)); // stop timer0 (disables timer IRQs)
    }

    uint16_t updateTimeout() const {
        uint16_t val = CAT2(ICR, CAP_TIM);
        CAT3(OCR, CAP_TIM, CAP_TIM_OC) = val; // timeout based on previous trigger time
        return val;
    }

    void static store(uint16_t* &pCapDat, uint32_t pulseTime, uint32_t gapTime);
#endif
    inline static uint16_t packTimeVal(uint32_t val) {
        if (val >= 0x8000) {
            val = val >> (RANGE_EXTENSION_BITS + 1);
            val |= 0x8000;
        }

        return val;
    }

    inline static uint32_t unpackTimeVal(uint32_t val) {
        if (val & 0x8000) {
            val = (val & 0x7fff) << (RANGE_EXTENSION_BITS + 1);
        }

        return val;
    }
};

#endif	/* IRWIDGETAGGREGATING_H */
