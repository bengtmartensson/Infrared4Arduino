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
 * @file ATmega2560.h
 *
 * @brief Hardware dependent definitions for boards based on the ATMega 2560.
 */

#pragma once

#include "avr.h"

#define CURRENT_CLASS ATmega2560

class ATmega2560 : public Board {
public:

    ATmega2560() {
    };

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
    }

    void timerDisableIntr() {
        TIMSK1 = 0;
    };



    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        const uint16_t pwmval = F_CPU / 2 / frequency;
        TCCR1A = _BV(WGM11);
        TCCR1B = _BV(WGM13) | _BV(CS10);
        ICR1 = pwmval;
        OCR1A = pwmval * dutyCycle / 100;
    };

    void timerConfigNormal() {
        TCCR1A = 0;
        TCCR1B = _BV(WGM12) | _BV(CS10);
        OCR1A = F_CPU * microsPerTick / 1000000;
        TCNT1 = 0;
    };

#define PWM_PIN 11

///////////////////////////////////////////////////////////////////////////////
#elif defined(IR_USE_TIMER2)
// Timer2 (8 bits)

#define TIMER_INTR_NAME     TIMER2_COMPA_vect

    void timerEnablePwm() {
        TCCR2A |= _BV(COM2B1);
    }

    void timerDisablePwm() {
        TCCR2A &= ~(_BV(COM2B1));
    }

    void timerEnableIntr() {
        TIMSK2 = _BV(OCIE2A);
    }

    void timerDisableIntr() {
        TIMSK2 = 0;
    }

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        const uint8_t pwmval = F_CPU / 2 / frequency;
        TCCR2A = _BV(WGM20);
        TCCR2B = _BV(WGM22) | _BV(CS20);
        OCR2A = pwmval;
        OCR2B = pwmval * dutyCycle / 100;
    };

#define TIMER_COUNT_TOP  (F_CPU * microsPerTick / 1000000UL)

    void timerConfigNormal() {
        //#if (TIMER_COUNT_TOP < 256)
        //    TCCR2A = _BV(WGM21);
        //    TCCR2B = _BV(CS20);
        //    OCR2A  = TIMER_COUNT_TOP;
        //    TCNT2  = 0;
        //#else
        TCCR2A = _BV(WGM21);
        TCCR2B = _BV(CS21);
        OCR2A = TIMER_COUNT_TOP / 8;
        TCNT2 = 0;
        //#endif
    };

#define PWM_PIN  9

//////////////////////////////////////////////////////////////////////////////
#elif defined(IR_USE_TIMER3)
// Timer3 (16 bits)
#define TIMER_INTR_NAME      TIMER3_COMPA_vect

    void timerEnablePwm() {
        TCCR3A |= _BV(COM3A1);
    };

    void timerDisablePwm() {
        TCCR3A &= ~(_BV(COM3A1));
    };

    void timerEnableIntr() {
        TIMSK3 = _BV(OCIE3A);
    };

    void timerDisableIntr() {
        TIMSK3 = 0;
    };

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        const uint16_t pwmval = F_CPU / 2 / frequency;
        TCCR3A = _BV(WGM31);
        TCCR3B = _BV(WGM33) | _BV(CS30);
        ICR3 = pwmval;
        OCR3A = pwmval * dutyCycle / 100;
    };

    void timerConfigNormal() {
        TCCR3A = 0;
        TCCR3B = _BV(WGM32) | _BV(CS30);
        OCR3A = F_CPU * microsPerTick / 1000000;
        TCNT3 = 0;
    };

#define PWM_PIN 5

///////////////////////////////////////////////////////////////////////////////
#elif defined(IR_USE_TIMER4)
// Timer4 (16 bits)
#define TIMER_INTR_NAME     TIMER4_COMPA_vect

    void timerEnablePwm() {
        TCCR4A |= _BV(COM4A1);
    };

    void timerDisablePwm() {
        TCCR4A &= ~(_BV(COM4A1));
    };

    void timerEnableIntr() {
        TIMSK4 = _BV(OCIE4A);
    };

    void timerDisableIntr() {
        TIMSK4 = 0;
    };

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        const uint16_t pwmval = F_CPU / 2 / frequency;
        TCCR4A = _BV(WGM41);
        TCCR4B = _BV(WGM43) | _BV(CS40);
        ICR4 = pwmval;
        OCR4A = pwmval * dutyCycle / 100;
    };

    void timerConfigNormal() {
        TCCR4A = 0;
        TCCR4B = _BV(WGM42) | _BV(CS40);
        OCR4A = F_CPU * microsPerTick / 1000000;
        TCNT4 = 0;
    };

#define PWM_PIN 6

///////////////////////////////////////////////////////////////////////////////
#elif defined(IR_USE_TIMER5)
// Timer5 (16 bits)
#define TIMER_INTR_NAME     TIMER5_COMPA_vect

    void timerEnablePwm() {
        TCCR5A |= _BV(COM5A1);
    };

    void timerDisablePwm() {
        TCCR5A &= ~(_BV(COM5A1));
    };

    void timerEnableIntr() {
        TIMSK5 = _BV(OCIE5A);
    };

    void timerDisableIntr() {
        TIMSK5 = 0;
    };

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        const uint16_t pwmval = F_CPU / 2 / frequency;
        TCCR5A = _BV(WGM51);
        TCCR5B = _BV(WGM53) | _BV(CS50);
        ICR5 = pwmval;
        OCR5A = pwmval * dutyCycle / 100;
    };

    void timerConfigNormal() {
        TCCR5A = 0;
        TCCR5B = _BV(WGM52) | _BV(CS50);
        OCR5A = F_CPU * microsPerTick / 1000000;
        TCNT5 = 0;
    };

#define PWM_PIN 46

///////////////////////////////////////////////////////////////////////////////
#else // ! defined(IR_USE_TIMER2)

#error Config error, either IR_USE_TIMER1, IR_USE_TIMER2, IR_USE_TIMER3, IR_USE_TIMER4, or IR_USE_TIMER5 must be defined.

#endif
};

/* From http://busyducks.com/ascii-art-arduinos                              +-----+

         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |           GND/RST2  [ ] [ ]         |
         |         MOSI2/SCK2  [ ] [ ]  SCL[ ] |   D0
         |            5V/MISO2 [ ] [ ]  SDA[ ] |   D1
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ]~|   B7
         | [ ]v.ref                 MISO/12[ ]~|   B6
         | [ ]RST                   MOSI/11[ ]~|   B5
         | [ ]3V3      +----------+      10[ ]~|   B4
         | [ ]5v       | ARDUINO  |       9[ ]~|   H6
         | [ ]GND      |   MEGA   |       8[ ]~|   H5
         | [ ]GND      +----------+            |
         | [ ]Vin                         7[ ]~|   H4
         |                                6[ ]~|   H3
         | [ ]A0                          5[ ]~|   E3
         | [ ]A1                          4[ ]~|   G5
         | [ ]A2                     INT5/3[ ]~|   E5
         | [ ]A3                     INT4/2[ ]~|   E4
         | [ ]A4                       TX>1[ ]~|   E1
         | [ ]A5                       RX<0[ ]~|   E0
         | [ ]A6                               |
         | [ ]A7                     TX3/14[ ] |   J1
         |                           RX3/15[ ] |   J0
         | [ ]A8                     TX2/16[ ] |   H1
         | [ ]A9                     RX2/17[ ] |   H0
         | [ ]A10               TX1/INT3/18[ ] |   D3
         | [ ]A11               RX1/INT2/19[ ] |   D2
         | [ ]A12           I2C-SDA/INT1/20[ ] |   D1
         | [ ]A13           I2C-SCL/INT0/21[ ] |   D0
         | [ ]A14                              |
         | [ ]A15                              |   Ports:
         |                RST SCK MISO         |    22=A0  23=A1
         |         ICSP   [ ] [ ] [ ]          |    24=A2  25=A3
         |                [ ] [ ] [ ]          |    26=A4  27=A5
         |                GND MOSI 5V          |    28=A6  29=A7
         | G                                   |    30=C7  31=C6
         | N 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 5 |    32=C5  33=C4
         | D 2 0 8 6 4 2 0 8 6 4 2 0 8 6 4 2 V |    34=C3  35=C2
         |         ~ ~                         |    36=C1  37=C0
         | @ # # # # # # # # # # # # # # # # @ |    38=D7  39=G2
         | @ # # # # # # # # # # # # # # # # @ |    40=G1  41=G0
         |           ~                         |    42=L7  43=L6
         | G 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 5 |    44=L5  45=L4
         | N 3 1 9 7 5 3 1 9 7 5 3 1 9 7 5 3 V |    46=L3  47=L2
         | D                                   |    48=L1  49=L0    SPI:
         |                                     |    50=B3  51=B2     50=MISO 51=MOSI
         |     2560                ____________/    52=B1  53=B0     52=SCK  53=SS
          \_______________________/

*/
