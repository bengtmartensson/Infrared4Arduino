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
 * @file ATmega32U4.h
 *
 * @brief Hardware dependent definitions for Leonardo like boards like
 * Leonardo and Micro.
 */

#pragma once

#include "avr.h"

#if !defined(ARDUINO_AVR_LEONARDO) && !defined(ARDUINO_AVR_MICRO)
#error Board not supported
#endif

#define CURRENT_CLASS ATmega32U4

class ATmega32U4 : public Board {
public:
    ATmega32U4() {};

private:

///////////////////////////////////////////////////////////////////////////////
#if defined(IR_USE_TIMER1)
// Timer1 (16 bits)
#define TIMER_INTR_NAME       TIMER1_COMPA_vect

    void timerEnablePwm() {
        TCCR1A |= _BV(COM1A1);
    };

    void timerDisablePwm() {
        TCCR1A &= ~(_BV(COM1A1));
    };

    void timerEnableIntr() {
        TIMSK1 = _BV(OCIE1A);
    };

    void timerDisableIntr() {
        TIMSK1 = 0U;
    };

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        const uint16_t pwmval = F_CPU / 2UL / frequency;
        TCCR1A = _BV(WGM11);
        TCCR1B = _BV(WGM13) | _BV(CS10);
        ICR1 = pwmval;
        OCR1A = pwmval * dutyCycle / 100UL;
    };

    void timerConfigNormal() {
        TCCR1A = 0;
        TCCR1B = _BV(WGM12) | _BV(CS10);
        OCR1A = F_CPU * microsPerTick / 1000000UL;
        TCNT1 = 0U;
    };

#define PWM_PIN  9

///////////////////////////////////////////////////////////////////////////////
#elif defined(IR_USE_TIMER2) // ! defined(IR_USE_TIMER1)
#define TIMER_INTR_NAME     TIMER2_COMPA_vect

#error IR_USE_TIMER2 in Leonardo currently broken.

void timerEnablePwm() {TCCR2A |= _BV(COM2B1);};
void timerDisablePwm() { TCCR2A &= ~(_BV(COM2B1));};
void timerEnableIntr() {TIMSK2 = _BV(OCIE2A);};
void timerDisableIntr() {TIMSK2 = 0;};

void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
    const uint8_t pwmval = F_CPU / 2U / frequency;
    TCCR2A               = _BV(WGM20);
    TCCR2B               = _BV(WGM22) | _BV(CS20);
    OCR2A                = pwmval;
    OCR2B                = pwmval * dutyCycle / 100UL; \
})

#define TIMER_COUNT_TOP  (F_CPU * microsPerTick / 1000000)

void timerConfigNormal() {
    TCCR2A = _BV(WGM21);
    TCCR2B = _BV(CS21);
    OCR2A  = TIMER_COUNT_TOP / 8U;
    TCNT2  = 0U;
};

#define PWM_PIN  3

#else // ! defined(IR_USE_TIMER2)

#error Config error, either IR_USE_TIMER1 or IR_USE_TIMER2 must be defined.

#endif
};

/* http://busyducks.com/ascii-art-arduinos

                  D0   D1   RST
   GND  GND  VCC  RX   TX   /DTR
        +--------------------------------+
        |  [ ]  [ ]  [ ]  [ ]  [ ]  [ ]  |
        |              FTDI              |
    D1  | [ ]1/TX                 RAW[ ] |
    D0  | [ ]0/RX                 GND[ ] |
        | [ ]RST        SCL/A5[ ] RST[ ] |   C6
        | [ ]GND        SDA/A4[ ] VCC[ ] |
    D2  | [ ]2/INT0    ___         A3[ ] |   C3
    D3  |~[ ]3/INT1   /   \        A2[ ] |   C2
    D4  | [ ]4       /PRO  \       A1[ ] |   C1
    D5  |~[ ]5       \ MINI/       A0[ ] |   C0
    D6  |~[ ]6        \___/    SCK/13[ ] |   B5
    D7  | [ ]7          A7[ ] MISO/12[ ] |   B4
    B0  | [ ]8          A6[ ] MOSI/11[ ]~|   B3
    B1  |~[ ]9                  SS/10[ ]~|   B2
        |           [RST-BTN]            |
        +--------------------------------+
*/