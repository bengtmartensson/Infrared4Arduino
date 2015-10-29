/*
Copyright (C) 2015 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#include <Arduino.h>
#include "IrSenderPwm.h"

// Copied from IRLib.c, necessary for the timing wizardry to work.
// Provides ISR
#ifdef ARDUINO
#include <avr/interrupt.h>
#endif
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#define CLKFUDGE 5      // fudge factor for clock interrupt overhead
#ifdef F_CPU
#define SYSCLOCK F_CPU     // main Arduino clock
#else
#define SYSCLOCK 16000000  // main Arduino clock
#endif
#define PRESCALE 8      // timer clock prescale
#define CLKSPERUSEC (SYSCLOCK/PRESCALE/1000000)   // timer clocks per microsecond

// From IRLib, contains the timing wizardry
#include <IRLibTimer.h>

IrSenderPwm *IrSenderPwm::instance = NULL;

IrSenderPwm::IrSenderPwm() : IrSender(IR_SEND_PWM_PIN) {
}

void IrSenderPwm::send(const IrSequence& irSequence, frequency_t frequency) {
    enable(frequency/1000);
    for (unsigned int i = 0; i < irSequence.getLength(); i++) {
        digitalWrite(getOutputPin(), (i & 1) ? LOW : HIGH);
        if (i & 1) {
            IR_SEND_PWM_STOP;
        } else {
            IR_SEND_PWM_START;
        }
        delayUSecs(irSequence.getDurations()[i]);
    }
    digitalWrite(getOutputPin(), LOW);
}

IrSenderPwm *IrSenderPwm::newInstance() {
    if (instance != NULL)
        return NULL;
    instance = new IrSenderPwm();
    return instance;
}

IrSenderPwm *IrSenderPwm::getInstance(boolean create) {
    if (instance == NULL && create)
        instance = new IrSenderPwm();
    return instance;
}

//void IrSenderPwm::mute() {
//    pinMode(IR_SEND_PWM_PIN, OUTPUT);
//    digitalWrite(IR_SEND_PWM_PIN, LOW);
//}

// Copied from IrLib.c, renamed from IRsendBase::enableIROut
#ifndef UNUSED
#define UNUSED
#endif
void IrSenderPwm::enable(unsigned char khz UNUSED) {
    //NOTE: the comments on this routine accompanied the original early version of IRremote library
    //which only used TIMER2. The parameters defined in IRLibTimer.h may or may not work this way.
    // Enables IR output.  The khz value controls the modulation frequency in kilohertz.
    // The IR output will be on pin 3 (OC2B).
    // This routine is designed for 36-40KHz; if you use it for other values, it's up to you
    // to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
    // TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
    // controlling the duty cycle.
    // There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
    // To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
    // A few hours staring at the ATmega documentation and this will all make sense.
    // See my Secrets of Arduino PWM at http://www.righto.com/2009/07/secrets-of-arduino-pwm.html for details.

    // Disable the Timer2 Interrupt (which is used for receiving IR)
    IR_RECV_DISABLE_INTR; //Timer2 Overflow Interrupt
    pinMode(IR_SEND_PWM_PIN, OUTPUT);
    digitalWrite(IR_SEND_PWM_PIN, LOW); // When not sending PWM, we want it low
    IR_SEND_CONFIG_KHZ(khz);
}
