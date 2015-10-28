// Copyright (c) 2012 Michael Dreher  <michael(at)5dot1.de>
// this code may be distributed under the terms of the General Public License V2 (GPL V2)

// This is a slight reorganization of the original code, by Bengt Martensson.

#ifndef IRWIDGETAGGREGATING_H
#define	IRWIDGETAGGREGATING_H

#include "IrWidget.h"

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
