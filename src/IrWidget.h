/* IR Widget: capture a raw IR signal and dump the timing of the non-demodulated signal

http://www.piclist.com/images/boards/irwidget/index.htm
http://www.hifi-remote.com/forums/dload.php?action=file&file_id=2044
http://www.hifi-remote.com/wiki/index.php?title=IR_Scope_and_IR_Widget_User%27s_Guide
http://www.compendiumarcana.com/irwidget/

Arduino digital pin numbers for the input capture pin (ICP) and the logic analyzer debugging pin (LA Dbg):
Board name / MCU                           | ICP pin                  | LA Dbg pin
-------------------------------------------|--------------.-----------|------------------------
Duemilanove/Uno (ATmega328P / ATmega168)   | ICP1/PB0, Arduino pin 8  | PD6, Arduino pin 6
Leonardo (ATmega32U4)                      | ICP1/PD4, Arduino pin 4  | PD6, Arduino pin 12
Arduino Mega 2560 (ATmega2560)             | ICP4/PL0, Arduino pin 49 | PL6, Arduino pin 43

see also here:
http://arduino.cc/en/Hacking/PinMapping168 (also for ATmega328P)
http://arduino.cc/en/Hacking/PinMapping32u4
http://arduino.cc/en/Hacking/PinMapping2560
 */

// Copyright (c) 2012 Michael Dreher  <michael(at)5dot1.de>
// this code may be distributed under the terms of the General Public License V2 (GPL V2)

// Code slighty reorganized by Bengt Martensson

//#define ALTERNATE_PIN
#pragma once

#include <Arduino.h>
#include "IrReader.h"
#include "Board.h"

#define ENABLE_PULL_UP
//#define DEBUG_PORT D
//#define DEBUG_PIN 6
//#define DEBUG_PORT L

#define USE_PRESCALER_FACTOR_8 1

/**
 * Base class for classes based upon ICP pins capture.
 * See <a href="http://www.mikrocontroller.net/articles/High-Speed_capture_mit_ATmega_Timer">this article (in German)</a>.
 */
class IrWidget : public IrReader {
public:
    static constexpr int16_t defaultMarkExcess = 0;

protected:
    frequency_t frequency;

    IrWidget(size_t captureLength = defaultCaptureLength,
            bool pullup = false,
            int16_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);
    virtual ~IrWidget();

public:
    /**
     * Set true means if sensor signal is inverted (low = signal on)
     * (false has not been tested, and is not supported).
     */
    static const bool invertingSensor = true;

    virtual void capture() = 0;

    /**
     *  For compatibility with the receiver classes, receive is a synonym for capture.
     */
    void receive() {
        capture();
    }

    size_t getDataLength() const { // was: getCaptureCount()
        return captureCount;
    }

    bool isReady() const {
        return timeouted || !isEmpty();
    }

    void reset() {
        captureCount = 0;
    }

    microseconds_t inline getDuration(unsigned int i) const {
        uint32_t result32 = timerValueToNanoSeconds(unpackTimeVal(captureData[i])) / 1000
                            + (i & 1 ? markExcess : -markExcess);
        return result32 <= MICROSECONDS_T_MAX ? (microseconds_t) result32 : MICROSECONDS_T_MAX;
    }

    /**
     * Sets the ending timeout. In this implementation, this is effectively
     * rounded to the nearest multiple of 32 milliseconds.
     * @param timeout timeout in milliseconds.
     */
    void setEndingTimeout(milliseconds_t timeout);

    milliseconds_t getEndingTimeout() const;

    frequency_t getFrequency() const {
        return frequency;
    }

    void dump(Stream &stream) const;

private:
    void setup(bool setup);

    ////////////////////////////////////////////////////////////////////////////////
    // Internal defines, don't change
    ////////////////////////////////////////////////////////////////////////////////

protected:
#if USE_PRESCALER_FACTOR_8
#define CAPTURE_PRESCALER_SETTING (_BV(CAT3(CS, CAP_TIM, 1)))
#define CAPTURE_PRESCALER_BITS (3)
#else
#define CAPTURE_PRESCALER_SETTING (_BV(CAT3(CS, CAP_TIM, 0)))
#define CAPTURE_PRESCALER_BITS (0)
#endif
#define CAPTURE_PRESCALER_FACTOR (_BV(CAPTURE_PRESCALER_BITS))

#if RANGE_EXTENSION_BITS > 8
    typedef uint16_t ovlBitsDataType;
#else
    typedef uint8_t ovlBitsDataType;
#endif

    static constexpr uint8_t RANGE_EXTENSION_BITS = 4; // factor for upper measurement range = 2^(RANGE_EXTENSION_BITS+1)

    ovlBitsDataType endingTimeout; // = _BV(RANGE_EXTENSION_BITS) - 1;

private:
    ////////////////////////////////////////////////////////////////////////////////
    // Adaption to different MCUs and clk values
    ////////////////////////////////////////////////////////////////////////////////
#if defined(_AVR_IOM32U4_H_)
#ifndef ALTERNATE_PIN
    // Digital pin 4, ICP1
#define CAP_PORT D
#define CAP_PIN 4
#define CAP_TIM 1
#define CAP_TIM_OC A
#else // Alternate pin
    // Digital pin 13, ICP3
#define CAP_PORT C
#define CAP_PIN 7
#define CAP_TIM 3
#define CAP_TIM_OC A
#endif // ALTERNATE_PIN
#elif defined(_AVR_IOM2560_H_)
#ifndef ALTERNATE_PIN
    // Digital pin 49, ICP4
#define CAP_PORT L
#define CAP_PIN 0
#define CAP_TIM 4
#define CAP_TIM_OC A
#else // ALTERNATE_PIN
    // Digital pin 48, ICP5
#define CAP_PORT L
#define CAP_PIN 1
#define CAP_TIM 5
#define CAP_TIM_OC A
#endif // ALTERNATE_PIN
#else
    // the default is the setting for the ATmega328P / ATmega168
    // Digital pin 8, ICP1
#define CAP_PORT B
#define CAP_PIN 0
#define CAP_TIM 1
#define CAP_TIM_OC A
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // Helper macros
    ////////////////////////////////////////////////////////////////////////////////
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  // set bit
#define __CAT2(base, portname) base##portname // internally needed by CAT2
#define CAT2(prefix, num) __CAT2(prefix, num) // build a define name from 2 params
#define __CAT3(prefix, num, postfix) prefix##num##postfix // internally needed by CAT3
#define CAT3(prefix, num, postfix) __CAT3(prefix, num, postfix) // build a define name from 3 params

    // these macros are used to debug the timing with an logic analyzer or oscilloscope on a port pin
protected:
    inline void debugPinToggle(void) {
#if defined(DEBUG_PIN) && defined(DEBUG_PORT)
        CAT2(PIN, DEBUG_PORT) = _BV(DEBUG_PIN);
#endif
    }

    inline void debugPinClear(void) {
#if defined(DEBUG_PIN) && defined(DEBUG_PORT)
        cbi(CAT2(PORT, DEBUG_PORT), DEBUG_PIN);
#endif
    }
    uint16_t *captureData; //[bufSize]; // the buffer where the catured data is stored
    uint16_t captureCount; // number of values stored in captureData
    static constexpr uint8_t sampleSize = 2;

    virtual uint32_t unpackTimeVal(uint32_t val) const = 0;

    // convert number of clocks to nanoseconds, try to use integer arithmetic and avoid
    // overflow and too much truncation (double arithmetic costs additional 800 byte of code)
    static uint32_t inline timerValueToNanoSeconds(uint32_t x) {
#if (F_CPU % 8000000) == 0
        return (x * (125UL << CAPTURE_PRESCALER_BITS)) / (F_CPU / 8000000UL);
#elif (F_CPU % 1000000) == 0
        return (x * (1000UL << CAPTURE_PRESCALER_BITS)) / (F_CPU / 1000000UL);
#elif (F_CPU % 115200) == 0 // serial bps rate compatible cpu clocks, e.g. 7372800 or 14745600
        // TODO: this has to be tested, especially the accuracy
        return (((x * (1000UL << CAPTURE_PRESCALER_BITS)) / (F_CPU / 115200UL)) * 625UL) / 72UL;
#else
        // TODO: this has to be tested
        return (uint32_t) (((double) x * (1.0E9 * (double) CAPTURE_PRESCALER_FACTOR)) / (double) F_CPU); // use double precision floating point arithmetic
#endif
    }
};
