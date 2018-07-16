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

protected:
    IrWidgetAggregating(size_t captureLength = defaultCaptureLength,
            bool pullup = false,
            int16_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

private:
    bool waitForFirstEdge();

    void inline setupTriggerAndTimers();

    bool inline overflowBit(uint8_t tifr) {
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

    void store(uint16_t* &pCapDat, uint32_t pulseTime, uint32_t gapTime);

    inline uint16_t packTimeVal(uint32_t val) const {
        if (val >= 0x8000) {
            val = val >> (RANGE_EXTENSION_BITS + 1);
            val |= 0x8000;
        }

        return val;
    }

    inline uint32_t unpackTimeVal(uint32_t val) const {
        if (val & 0x8000) {
            val = (val & 0x7fff) << (RANGE_EXTENSION_BITS + 1);
        }

        return val;
    }
};

#endif	/* IRWIDGETAGGREGATING_H */
