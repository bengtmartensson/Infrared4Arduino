// Copyright (c) 2012 Michael Dreher  <michael(at)5dot1.de>
// this code may be distributed under the terms of the General Public License V2 (GPL V2)

// This is a slight reorganization of the original code, by Bengt Martensson.

#pragma once

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

    void endCapture();
    bool waitForFirstEdge();
    uint8_t waitForEdgeOrOverflow();
    void saveTimerIrq();
    void restoreTimerIrq();
    uint8_t computeTccrnb() const;
    void storePulse(uint32_t onTime, uint32_t offTime);
    bool isOverflow(uint8_t tifr) const;
    bool isInputCaptureEvent(uint8_t tifr) const;
    void clearInputCaptureOutputCompare();

    uint8_t tccr0b;
    uint8_t sreg;
    //uint16_t aggThreshold;

    static const frequency_t min_frequency = 20000U;
    static const uint16_t period = (F_CPU / min_frequency) >> CAPTURE_PRESCALER_BITS; // the time of one period in CPU clocks

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

    class FrequencyCalculator {
    private:
        uint8_t calShiftM1;// = 1;
        uint8_t calCount;// = 1 << (calShiftM1 + 1);
        uint8_t aggCount;// = 0;
        uint16_t aggThreshold;
    public:
        frequency_t getFrequency() const;
        FrequencyCalculator() { init(); };
        void disable() { calCount = 0U; }
        uint32_t compute(uint32_t aggVal);
        void init();
        uint16_t getAggThreshold() const { return aggThreshold ; }
    };

    FrequencyCalculator frequencyCalculator;
};