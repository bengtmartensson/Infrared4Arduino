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

#if !defined(ARDUINO_AVR_LEONARDO) && !defined(ARDUINO_AVR_MICRO)
#error Board not supported
#endif

#include "avr.h"

#define USE_DEFAULT_ENABLE_IR_IN
#define TIMER_RESET

#if defined(IR_USE_TIMER1)
// Timer1 (16 bits)

#define TIMER_ENABLE_PWM   (TCCR1A |= _BV(COM1A1))
#define TIMER_DISABLE_PWM  (TCCR1A &= ~(_BV(COM1A1)))

#define TIMER_ENABLE_INTR   (TIMSK1 = _BV(OCIE1A))
#define TIMER_DISABLE_INTR  (TIMSK1 = 0)

#define TIMER_INTR_NAME       TIMER1_COMPA_vect

#define TIMER_CONFIG_KHZ(val) ({ \
    const uint16_t pwmval = F_CPU / 2000 / (val); \
    TCCR1A                = _BV(WGM11); \
    TCCR1B                = _BV(WGM13) | _BV(CS10); \
    ICR1                  = pwmval; \
    OCR1A                 = pwmval / 3; \
})

#define TIMER_CONFIG_NORMAL() ({ \
    TCCR1A = 0; \
    TCCR1B = _BV(WGM12) | _BV(CS10); \
    OCR1A  = F_CPU * USECPERTICK / 1000000; \
    TCNT1  = 0; \
})

#define SEND_PIN  9

#elif defined(IR_USE_TIMER2) // ! defined(IR_USE_TIMER1)

#error IR_USE_TIMER2 in Leonardo currently broken.

#define TIMER_ENABLE_PWM    (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM   (TCCR2A &= ~(_BV(COM2B1)))
#define TIMER_ENABLE_INTR   (TIMSK2 = _BV(OCIE2A))
#define TIMER_DISABLE_INTR  (TIMSK2 = 0)
#define TIMER_INTR_NAME     TIMER2_COMPA_vect

#define TIMER_CONFIG_KHZ(val) ({ \
    const uint8_t pwmval = F_CPU / 2000 / (val); \
    TCCR2A               = _BV(WGM20); \
    TCCR2B               = _BV(WGM22) | _BV(CS20); \
    OCR2A                = pwmval; \
    OCR2B                = pwmval / 3; \
})

#define TIMER_COUNT_TOP  (F_CPU * USECPERTICK / 1000000)

#if (TIMER_COUNT_TOP < 256)
#define TIMER_CONFIG_NORMAL() ({ \
    TCCR2A = _BV(WGM21); \
    TCCR2B = _BV(CS20); \
    OCR2A  = TIMER_COUNT_TOP; \
    TCNT2  = 0; \
})
#else
#define TIMER_CONFIG_NORMAL() ({ \
    TCCR2A = _BV(WGM21); \
    TCCR2B = _BV(CS21); \
    OCR2A  = TIMER_COUNT_TOP / 8; \
    TCNT2  = 0; \
})
#endif

#define SEND_PIN  3

#else // ! defined(IR_USE_TIMER2)

#error Config error, either IR_USE_TIMER1 or IR_USE_TIMER2 must be defined.

#endif

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