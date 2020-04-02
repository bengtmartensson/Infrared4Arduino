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
 * @file ATmega328P.h
 *
 * @brief Hardware dependent definitions for boards based on the ATMega 328P.
 * This includes the Arduino Uno, Nano, and many others.
 */

#pragma once

#include "avr.h"

#define CURRENT_CLASS ATmega328P

class ATmega328P : public Board {
public:

/////////////////////////////////////////////////////////////
#ifdef IR_USE_TIMER1

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

#define TIMER_INTR_NAME       TIMER1_COMPA_vect

    void timerConfigHz(frequency_t val, dutycycle_t dutyCycle) {
        const uint16_t pwmval = F_CPU / 2U / val;
        TCCR1A = _BV(WGM11);
        TCCR1B = _BV(WGM13) | _BV(CS10);
        ICR1 = pwmval;
        OCR1A = pwmval * dutyCycle / 100U;
    };

    void timerConfigNormal() {
        TCCR1A = 0U;
        TCCR1B = _BV(WGM12) | _BV(CS10);
        OCR1A = F_CPU * microsPerTick / 1000000UL;
        TCNT1 = 0U;
    };

#define PWM_PIN  9
    //////////////////////////////////////////////////////////////////////////
#elif defined(IR_USE_TIMER2) || defined(DOXYGEN) // ! defined(IR_USE_TIMER1)

    void timerEnablePwm() {
        TCCR2A |= _BV(COM2B1);
    }

    void timerDisablePwm() {
        TCCR2A &= ~(_BV(COM2B1));
    }

    void timerEnableIntr() {
        TIMSK2 = _BV(OCIE2A);
    };

    void timerDisableIntr() {
        TIMSK2 = 0U;
    };

#define TIMER_INTR_NAME     TIMER2_COMPA_vect

    void timerConfigHz(frequency_t val, dutycycle_t dutyCycle) {
        const uint8_t pwmval = F_CPU / 2U / val;
        TCCR2A = _BV(WGM20);
        TCCR2B = _BV(WGM22) | _BV(CS20);
        OCR2A = pwmval;
        OCR2B = pwmval * dutyCycle / 100U;
    };

    void timerConfigNormal() {
        TCCR2A = _BV(WGM21);
        TCCR2B = _BV(CS21);
        OCR2A = F_CPU * microsPerTick / 1000000UL / 8U;
        TCNT2 = 0U;
    }
/////////////////////////////////////////////////////////////////////////

#define PWM_PIN  3

#else // ! defined(IR_USE_TIMER2)

#error Config error, either IR_USE_TIMER1 or IR_USE_TIMER2 must be defined.

#endif
};

/* From http://busyducks.com/ascii-art-arduinos



         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |         GND/RST2  [ ][ ]            |
         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5
         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ] |   B5
         | [ ]v.ref                 MISO/12[ ] |   .
         | [ ]RST                   MOSI/11[ ]~|   .
         | [ ]3V3    +---+               10[ ]~|   .
         | [ ]5v     | A |                9[ ]~|   .
         | [ ]GND   -| R |-               8[ ] |   B0
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[ ] |   D7
         |          -| I |-               6[ ]~|   .
         | [ ]A0    -| N |-               5[ ]~|   .
         | [ ]A1    -| O |-               4[ ] |   .
         | [ ]A2     +---+           INT1/3[ ]~|   .
         | [ ]A3                     INT0/2[ ] |   .
         | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   .
         | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0
         |            [ ] [ ] [ ]              |
         |  UNO_R3    GND MOSI 5V  ____________/
          \_______________________/


                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[ ] |   B4
         | [ ]3.3V           MOSI/D11[ ]~|   B3
         | [ ]V.ref     ___    SS/D10[ ]~|   B2
    C0   | [ ]A0       / N \       D9[ ]~|   B1
    C1   | [ ]A1      /  A  \      D8[ ] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[ ]~|   D6
    C4   | [ ]A4/SDA               D5[ ]~|   D5
    C5   | [ ]A5/SCL               D4[ ] |   D4
         | [ ]A6              INT1/D3[ ]~|   D3
         | [ ]A7              INT0/D2[ ] |   D2
         | [ ]5V                  GND[ ] |
    C6   | [ ]RST                 RST[ ] |   C6
         | [ ]GND   5V MOSI GND   TX1[ ] |   D0
         | [ ]Vin   [ ] [ ] [ ]   RX1[ ] |   D1
         |          [ ] [ ] [ ]          |
         |          MISO SCK RST         |
         | NANO-V3                       |
         +-------------------------------+

*/
