/*
Copyright (C) 2020 Bengt Martensson.

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

/**
 * @file ATmega4809.h
 *
 * @brief Hardware dependent definitions for boards based on the ATMega 4809.
 * This includes the Arduino Uno WiFi Rev. 2, Nano Every.
 *
 * This file is based upon https://github.com/z3t0/Arduino-IRremote/pull/657,
 * incorporating https://github.com/z3t0/Arduino-IRremote/pull/657#issuecomment-570900315
 * (second version) and
 * https://github.com/z3t0/Arduino-IRremote/pull/657#issuecomment-571841983 .
 */

#pragma once

#define CURRENT_CLASS ATmega4809

#define HAS_FLASH_READ      1
#define HAS_HARDWARE_PWM    1
#define HAS_SAMPLING        1
#define HAS_INPUT_CAPTURE   0

#define STRCPY_PF_CAST(x) (x)

class ATmega4809 : public Board {
public:
    ATmega4809() {
    };

private:

#ifdef IR_USE_TIMER1
///////////////////////////////////////////////////////////////////////////////
#define TIMER_INTR_NAME      TCB0_INT_vect

    void timerReset() {
        TCB0.INTFLAGS = TCB_CAPT_bm;
    };
    //#define TIMER_ENABLE_PWM     (TCB0.CTRLB = TCB_CNTMODE_PWM8_gc)
    //#define TIMER_DISABLE_PWM    (TCB0.CTRLB &= ~(TCB_CNTMODE_PWM8_gc))

    void timerEnablePwm() {
        TCB0.CTRLB |= TCB_CCMPEN_bm;
    };

    void timerDisablePwm() {
        TCB0.CTRLB &= ~(TCB_CCMPEN_bm);
    };

    void timerEnableIntr() {
        TCB0.INTCTRL = TCB_CAPT_bm;
    };

    void timerDisableIntr() {
        TCB0.INTCTRL &= ~TCB_CAPT_bm;
    };

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        const uint8_t pwmval = F_CPU / 2U / frequency;
        TCB0.CTRLB = TCB_CNTMODE_PWM8_gc;
        TCB0.CCMPL = pwmval;
        TCB0.CCMPH = pwmval * dutyCycle / 100UL;
        TCB0.CTRLA = TCB_CLKSEL_CLKDIV2_gc | TCB_ENABLE_bm;
    };

    void timerConfigNormal() {
        TCB0.CTRLB = TCB_CNTMODE_INT_gc;
        TCB0.CCMP = F_CPU * microsPerTick / 1000000UL;
        TCB0.INTCTRL = TCB_CAPT_bm;
        TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm;
    };

#define PWM_PIN  6

///////////////////////////////////////////////////////////////////////////////
#elif defined(IR_USE_TIMER2) // ! defined(IR_USE_TIMER1)

// TODO... ;-)
#error IR_USE_TIMER2 for this architecture is not yet implemented, sorry.

#else // ! defined(IR_USE_TIMER2)

#error Config error, either IR_USE_TIMER1 or IR_USE_TIMER2 must be defined.

#endif
};