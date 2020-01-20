/*
Copyright (C) 2019 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#include <Arduino.h>
#include "IrSenderSoftCarrier.h"

//#define wait(t) delayMicroseconds(t)
inline void wait(unsigned long t) {
    unsigned long end = micros() + t;
    while (micros() < end)
#if ! defined(ARDUINO) && ! defined(REAL_TIME)
        // increment the simulated time, otherwise will loop forever
        delayMicroseconds(1)
#endif
        ;
}

IrSenderSoftCarrier::IrSenderSoftCarrier(pin_t sendPin) : IrSender(sendPin) {
}

void IrSenderSoftCarrier::send(const IrSequence& irSequence, frequency_t frequency) {
    enable(frequency);
    unsigned long start = micros();
    for (unsigned int i = 0; i < irSequence.getLength(); i += 2) {
        mark(irSequence.getDurations()[i]); // leaves LOW
        unsigned long ending = start + irSequence.getDurations()[i] + irSequence.getDurations()[i + 1];
        milliseconds_t remain = ending > start /* not overflow */ ? (microseconds_t) (ending - micros()) : irSequence.getDurations()[i + 1];
        wait(remain);
        start = ending;
    }
}

//+=============================================================================
// Enables IR output.  The khz value controls the modulation frequency in kilohertz.
// The IR output will be on pin 3 (OC2B).
// This routine is designed for 36-40KHz; if you use it for other values, it's up to you
// to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
// TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
// controlling the duty cycle.
// There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
// To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
// A few hours staring at the ATmega documentation and this will all make sense.
// See my Secrets of Arduino PWM at http://arcfn.com/2009/07/secrets-of-arduino-pwm.html for details.
//
void IrSenderSoftCarrier::enable(frequency_t hz) {
    //TIMER_DISABLE_INTR;
    pinMode(getOutputPin(), OUTPUT);
    writeLow();
    //TIMER_CONFIG_KHZ(khz);
    periodTime = (1000000UL + hz / 2L) / hz; // = 1000000/hz + 1/2 = round(1000000.0/hz)
    periodOnTime = periodTime * dutyCycle / 100U /*- pulseCorrection*/;
    periodOffTime = periodTime - periodOnTime;
}

//void inline IrSenderSoftCarrier::sleepMicros(unsigned long us) {
//#ifdef USE_SPIN_WAIT
//    sleepUntilMicros(micros() + us);
//#else
//    if (us > 0U) // Is this necessary? (Official docu https://www.arduino.cc/en/Reference/DelayMicroseconds does not tell.)
//        delayMicroseconds(us);
//#endif
//}
//
//void inline IrSenderSoftCarrier::sleepUntilMicros(unsigned long targetTime) {
//#ifdef USE_SPIN_WAIT
//    while (micros() < targetTime)
//        ;
//#else
//    unsigned long now = micros();
//    if (now < targetTime)
//        sleepMicros(targetTime - now);
//#endif
//}

//+=============================================================================
// Sends an IR mark for the specified number of microseconds.
// The mark output is modulated at the PWM frequency.
//

void IrSenderSoftCarrier::mark(microseconds_t time) {
    unsigned long start = micros();
    unsigned long stop = start + time;
    if (stop + periodTime < start)
        // Counter wrap-around, happens very seldomly, but CAN happen.
        // Just give up instead of possibly damaging the hardware.
        return;

    //unsigned long nextPeriodEnding = start;
    unsigned long now = micros();
    while (now < stop) {
        writeHigh();
        wait(periodOnTime);
        writeLow();
        //nextPeriodEnding += periodTime;
        wait(periodOffTime);
        now = micros();
    }
}

//+=============================================================================
// Leave pin off for time (given in microseconds)
// Sends an IR space for the specified number of microseconds.
// A space is no output, so the PWM output is disabled.
//
//
//void IrSenderSoftCarrier::space(unsigned int time) {
//    TIMER_DISABLE_PWM; // Disable pin 3 PWM output
//    if (time > 0) IrSenderSoftCarrier::custom_delay_usec(time);
//}

//+=============================================================================
// Custom delay function that circumvents Arduino's delayMicroseconds limit

//void IrSenderSoftCarrier::custom_delay_usec(unsigned long uSecs) {
//    if (uSecs > 4) {
//        unsigned long start = micros();
//        unsigned long endMicros = start + uSecs - 4;
//        if (endMicros < start) { // Check if overflow
//            while (micros() > start) {
//            } // wait until overflow
//        }
//        while (micros() < endMicros) {
//        } // normal wait
//    }
//    //else {
//    //  __asm__("nop\n\t"); // must have or compiler optimizes out
//    //}
//}
