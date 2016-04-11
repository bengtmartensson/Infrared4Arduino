// Copyright (c) 2012 Michael Dreher  <michael(at)5dot1.de>
// this code may be distributed under the terms of the General Public License V2 (GPL V2)

// This is a slight reorganization of the original code, by Bengt Martensson.

#ifndef IRWIDGETAGGREGATING_H
#define	IRWIDGETAGGREGATING_H

#include "IrWidget.h"

#define TCCR_ CAT3(TCCR, CAP_TIM, B)
#define ICES_ CAT2(ICES, CAP_TIM)
#define TIFR_ CAT2(TIFR, CAP_TIM)
#define ICF_  CAT2(ICF, CAP_TIM)
#define OCF_  CAT3(OCF, CAP_TIM, CAP_TIM_OC)
#define ICR_  CAT2(ICR, CAP_TIM)
#define OCR_  CAT3(OCR, CAP_TIM, CAP_TIM_OC)

class IrWidgetAggregating : public IrWidget {
private:
    static IrWidgetAggregating *instance;

public:
    void capture();
    static void deleteInstance();

    static IrWidgetAggregating *getInstance() {
        return instance;
    }

    static IrWidgetAggregating *newIrWidgetAggregating(size_t captureLength = defaultCaptureLength,
            boolean pullup = false,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

protected:
    IrWidgetAggregating(size_t captureLength = defaultCaptureLength,
            boolean pullup = false,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

private:
    inline uint16_t packTimeVal/*Normal*/(uint32_t val) const {
        if (val >= 0x8000) {
            val = val >> (RANGE_EXTENSION_BITS + 1);
            val |= 0x8000;
        }

        return val;
    }

    inline uint32_t unpackTimeVal/*Normal*/(uint32_t val) const {
        if (val & 0x8000) {
            val = (val & 0x7fff) << (RANGE_EXTENSION_BITS + 1);
        }

        return val;
    }
};

#endif	/* IRWIDGETAGGREGATING_H */
