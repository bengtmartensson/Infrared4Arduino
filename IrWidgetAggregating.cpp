// Copyright (c) 2012 Michael Dreher  <michael(at)5dot1.de>
// this code may be distributed under the terms of the General Public License V2 (GPL V2)

// This is a slight reorganization of the original code, by Bengt Martensson.

#include "IrWidgetAggregating.h"

IrWidgetAggregating::IrWidgetAggregating(size_t captureLength,
        boolean pullup,
        milliseconds_t beginningTimeout,
        milliseconds_t endingTimeout)
: IrWidget(captureLength, pullup, beginningTimeout, endingTimeout) {
}

IrWidgetAggregating *IrWidgetAggregating::instance = NULL;

IrWidgetAggregating *IrWidgetAggregating::newIrWidgetAggregating(size_t captureLength,
            boolean pullup,
            milliseconds_t beginningTimeout,
            milliseconds_t endingTimeout) {
    if (instance != NULL)
        return NULL;
    instance = new IrWidgetAggregating(captureLength, pullup,
            beginningTimeout, endingTimeout);
    return instance;
}

void IrWidgetAggregating::deleteInstance() {
    delete instance;
    instance = NULL;
}

// Wait for a signal on pin ICP1 and store the captured time values in the array 'captureData'
void IrWidgetAggregating::capture() {
#ifdef ARDUINO
    uint32_t timeForBeginTimeout = millis() + beginningTimeout;
    uint8_t tccr0b = TCCR0B;
    //TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00)); // stop timer0 (disables timer IRQs)

    /*register uint16_t*/ period = ((F_CPU) / (20000UL)) >> CAPTURE_PRESCALER_BITS; // the time of one period in CPU clocks
    //register uint16_t aggThreshold = (period * 10UL) / 8UL; // 65 us = (1/20kHz * 130%) might be a good starting point
    register uint16_t aggThreshold = period * 2;
    register uint8_t icesn_val = _BV(ICES_);
    register uint8_t tccrnb = TCCR_;
    if (invertingSensor)
        tccrnb &= ~icesn_val; // trigger on falling edge
    else
        tccrnb |= icesn_val; // trigger on rising edge

    TCCR_ = tccrnb;
    OCR1A = CAT2(TCNT, CAP_TIM) - 1;
    TIFR_ = _BV(ICF_)
            | _BV(OCF_) | _BV(CAT2(TOV, CAP_TIM)); // clear all timer flags
    register uint8_t tifr; // cache the result of reading TIFR1 (masked with ICF1 and OCF1A)
    register uint8_t calShiftM1 = 1;
    register uint8_t calCount = 1 << (calShiftM1 + 1);
    register uint8_t aggCount = 0;
    register ovlBitsDataType ovlCnt = 0;
    register uint16_t val;
    register uint16_t prevVal = 0;
    register uint16_t *pCapDat = captureData; // pointer to current item in captureData[]
    register uint32_t aggVal = 0;
    register uint32_t diffVal;

    // disabling IRQs for a long time will disconnect the USB connection of the ATmega32U4, therefore we
    // defer the sbi() instruction until we got the starting edge and only stop the Timer0 in the meanwhile
    uint8_t sreg = SREG;
    debugPinClear();

    /////////////////////////////////////////
    // wait for first edge
    while (!(TIFR_ & _BV(ICF_))) {
        if (millis() >= timeForBeginTimeout) {
            timeouted = true;
            goto endCapture;
        }
        //if (stream.available()) // abort the capture when any character is received // FIXME
        //    goto endCapture;
    }
    TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00)); // stop timer0 (disables timer IRQs)
    debugPinToggle();
    val = ICR_;
    OCR_ = val; // timeout based on previous trigger time

    noInterrupts(); // disable IRQs after the first edge

    // clear input capture and output compare flag bit
    TIFR_ = _BV(ICF_) | _BV(OCF_);
    prevVal = val;

    /////////////////////////////////////////
    // wait for all following edges
    while (pCapDat <= &captureData[bufferSize - sampleSize]) // 2 values are stored in each loop, TODO: change to 3 when adding the aggCount
    {
        debugPinToggle();
        // wait for edge or overflow (output compare match)
        do {
            // Fetch the input capture and output compare flags.
            tifr = TIFR_ & (_BV(ICF_) | _BV(OCF_));
        } while (!tifr);
        // Clear the input capture and output compare flags if they were set.
        TIFR_ = tifr;
        debugPinToggle();
        val = ICR_;
        OCR_ = val; // timeout based on previous trigger time

        if (tifr & _BV(OCF_)) // check for overflow bit
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
            continue;
        }

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
    period = aggThreshold >> 1;

    uint32_t mediumPeriod = timerValueToNanoSeconds(period);
    frequency = (frequency_t) (1000000000L / mediumPeriod);
#endif // ARDUINO
}
