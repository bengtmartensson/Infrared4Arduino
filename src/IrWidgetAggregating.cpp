// Copyright (c) 2012 Michael Dreher  <michael(at)5dot1.de>
// this code may be distributed under the terms of the General Public License V2 (GPL V2)

// This is a slight reorganization of the original code, by Bengt Martensson.

#include "IrWidgetAggregating.h"

#if HAS_INPUT_CAPTURE

static constexpr frequency_t min_frequency = 20000U;

IrWidgetAggregating::IrWidgetAggregating(size_t captureLength,
        bool pullup,
        int16_t markExcess,
        milliseconds_t beginningTimeout,
        milliseconds_t endingTimeout)
: IrWidget(captureLength, pullup, markExcess, beginningTimeout, endingTimeout) {
}

IrWidgetAggregating *IrWidgetAggregating::instance = nullptr;

IrWidgetAggregating *IrWidgetAggregating::newIrWidgetAggregating(size_t captureLength,
            bool pullup,
            int16_t markExcess,
            milliseconds_t beginningTimeout,
            milliseconds_t endingTimeout) {
    if (instance != nullptr)
        return nullptr;
    instance = new IrWidgetAggregating(captureLength, pullup, markExcess,
            beginningTimeout, endingTimeout);
    return instance;
}

void IrWidgetAggregating::deleteInstance() {
    delete instance;
    instance = nullptr;
}

// Wait for a signal on pin ICP1 and store the captured time values in the array 'captureData'
void IrWidgetAggregating::capture() {
#ifdef ARDUINO
    uint32_t timeForBeginTimeout = millis() + beginningTimeout;
    uint8_t tccr0b = TCCR0B;
    //TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00)); // stop timer0 (disables timer IRQs)

    uint16_t period = (F_CPU / min_frequency) >> CAPTURE_PRESCALER_BITS; // the time of one period in CPU clocks
    //uint16_t aggThreshold = (period * 10UL) / 8UL; // 65 us = (1/20kHz * 130%) might be a good starting point
    uint16_t aggThreshold = period * 2U;
    uint8_t icesn_val = _BV(CAT2(ICES, CAP_TIM));
    uint8_t tccrnb = CAT3(TCCR, CAP_TIM, B);
    if (invertingSensor)
        tccrnb &= ~icesn_val; // trigger on falling edge
    else
        tccrnb |= icesn_val; // trigger on rising edge

    CAT3(TCCR, CAP_TIM, B) = tccrnb;
    OCR1A = CAT2(TCNT, CAP_TIM) - 1;
    CAT2(TIFR, CAP_TIM) = _BV(CAT2(ICF, CAP_TIM))
            | _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC)) | _BV(CAT2(TOV, CAP_TIM)); // clear all timer flags
    uint8_t tifr; // cache the result of reading TIFR1 (masked with ICF1 and OCF1A)
    uint8_t calShiftM1 = 1;
    uint8_t calCount = 1 << (calShiftM1 + 1);
    uint8_t aggCount = 0;
    ovlBitsDataType ovlCnt = 0;
    uint16_t val;
    uint16_t prevVal = 0;
    uint16_t *pCapDat = captureData; // pointer to current item in captureData[]
    uint32_t aggVal = 0;
    uint32_t diffVal;

    // disabling IRQs for a long time will disconnect the USB connection of the ATmega32U4, therefore we
    // defer the sbi() instruction until we got the starting edge and only stop the Timer0 in the meanwhile
    uint8_t sreg = SREG;
    debugPinClear();

    /////////////////////////////////////////
    // wait for first edge
    while (!(tifr = (CAT2(TIFR, CAP_TIM) & (_BV(CAT2(ICF, CAP_TIM)))))) {
        if (millis() >= timeForBeginTimeout) {
            timeouted = true;
            goto endCapture;
        }
    }
    TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00)); // stop timer0 (disables timer IRQs)
    debugPinToggle();
    val = CAT2(ICR, CAP_TIM);
    CAT3(OCR, CAP_TIM, CAP_TIM_OC) = val; // timeout based on previous trigger time

    noInterrupts(); // disable IRQs after the first edge

    // clear input capture and output compare flag bit
    CAT2(TIFR, CAP_TIM) = _BV(CAT2(ICF, CAP_TIM)) | _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC));
    prevVal = val;

    /////////////////////////////////////////
    // wait for all following edges
    for (; pCapDat <= &captureData[bufferSize - sampleSize];) // 2 values are stored in each loop, TODO: change to 3 when adding the aggCount
    {
        debugPinToggle();
        // wait for edge or overflow (output compare match)
        while (!(tifr =
                (CAT2(TIFR, CAP_TIM) & (_BV(CAT2(ICF, CAP_TIM)) | _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC)))))) {
        }
        debugPinToggle();
        val = CAT2(ICR, CAP_TIM);
        CAT3(OCR, CAP_TIM, CAP_TIM_OC) = val; // timeout based on previous trigger time

        if (tifr & _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC))) // check for overflow bit
        {
            if (ovlCnt >= endingTimeout) // TODO: handle this check together with the check for the pulse length (if packTimeValNormal can handle the value)
            {
                if (aggVal > 0) {
                    // TODO check is to value is small enough to be stored
                    *pCapDat = packTimeVal/*Normal*/(aggVal); // store the pulse length
                    pCapDat++;
                    *pCapDat = packTimeVal/*Normal*/((uint32_t) endingTimeout << 16);
                    pCapDat++;
                }
                break; // maximum value reached, treat this as timeout and abort capture
            }
            ovlCnt++;
            // clear input capture and output compare flag bit
            CAT2(TIFR, CAP_TIM) = _BV(CAT2(ICF, CAP_TIM)) | _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC));
            continue;
        }

        // clear input capture and output compare flag bit
        CAT2(TIFR, CAP_TIM) = _BV(CAT2(ICF, CAP_TIM)) | _BV(CAT3(OCF, CAP_TIM, CAP_TIM_OC));

        diffVal = ((val - prevVal) & 0xffff) | ((uint32_t) ovlCnt << 16);
        ovlCnt = 0;
        prevVal = val;

        if (diffVal < aggThreshold) {
            aggVal += diffVal;

            // calculate the carrier frequency only within the first burst (often a preamble)
            if (calCount) {
                aggCount++; // only used to calculate the period
                // do a calibration on every aggCount which is a power of two because then dividing by calShiftM1
                // (shiftcount - 1) can simply be performed by shifting right
                if (aggCount == calCount) {
                    aggThreshold = aggVal >> calShiftM1;
                    calShiftM1++;
                    calCount = calCount << 1; // this will automatically terminate calibrating when calCount is 128 because then (128 << 1) & 0xff = 0
                }
            }
        } else {
            *pCapDat = packTimeVal/*Normal*/(aggVal); // store the pulse length
            pCapDat++;
            // TODO check if value is small enough to be stored
            *pCapDat = packTimeVal/*Normal*/(diffVal); // store the pause length
            pCapDat++;

            aggVal = 0;
            calCount = 0; // avoid further period calculation and calibration
        }
    }

endCapture:
    debugPinClear();

    TCCR0B = tccr0b; // re-enable Timer0
    SREG = sreg; // enable IRQs

    captureCount = pCapDat - captureData;
    if (aggThreshold == period * 2U) {
        frequency = 0U;
    } else {
        uint32_t mediumPeriod = timerValueToNanoSeconds(aggThreshold / 2U);
        frequency = (frequency_t) (1000000000L / mediumPeriod);
    }
#endif // ARDUINO
}
#endif // TCCR0B
