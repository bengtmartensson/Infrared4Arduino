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

#include "avr.h"

#define USE_DEFAULT_ENABLE_IR_IN

#ifdef IR_USE_TIMER1

#define TIMER_RESET          TCB0.INTFLAGS = TCB_CAPT_bm
//#define TIMER_ENABLE_PWM     (TCB0.CTRLB = TCB_CNTMODE_PWM8_gc)
//#define TIMER_DISABLE_PWM    (TCB0.CTRLB &= ~(TCB_CNTMODE_PWM8_gc))
#define TIMER_ENABLE_PWM     (TCB0.CTRLB |= TCB_CCMPEN_bm)
#define TIMER_DISABLE_PWM    (TCB0.CTRLB &= ~(TCB_CCMPEN_bm))
#define TIMER_ENABLE_INTR    (TCB0.INTCTRL = TCB_CAPT_bm)
#define TIMER_DISABLE_INTR   (TCB0.INTCTRL &= ~(TCB_CAPT_bm))

#define TIMER_INTR_NAME      TCB0_INT_vect
#define TIMER_CONFIG_KHZ(val) ({ \
  const uint8_t pwmval = F_CPU / 2000 / (val); \
  TCB0.CTRLB = TCB_CNTMODE_PWM8_gc; \
  TCB0.CCMPL = pwmval; \
  TCB0.CCMPH = pwmval / 3; \
  TCB0.CTRLA = (TCB_CLKSEL_CLKDIV2_gc) | (TCB_ENABLE_bm); \
})
#define TIMER_COUNT_TOP      ((F_CPU * USECPERTICK / 1000000))
#define TIMER_CONFIG_NORMAL() ({ \
  TCB0.CTRLB = (TCB_CNTMODE_INT_gc); \
  TCB0.CCMP = TIMER_COUNT_TOP; \
  TCB0.INTCTRL = TCB_CAPT_bm; \
  TCB0.CTRLA = (TCB_CLKSEL_CLKDIV1_gc) | (TCB_ENABLE_bm); \
})

#define SEND_PIN  6

#elif defined(IR_USE_TIMER2) // ! defined(IR_USE_TIMER1)
// TODO... ;-)
#error IR_USE_TIMER2 is not yet implemented

#else // ! defined(IR_USE_TIMER2)

#error Config error, either IR_USE_TIMER1 or IR_USE_TIMER2 must be defined.

#endif
