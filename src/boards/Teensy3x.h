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
 * @file Teensy3x.h
 *
 * @brief Hardware dependent definitions for Teensy 3.1/3.2 and probably more.
 * Based upon boarddefs.h from IRremote.
 * Likely originally written by Paul Stoffregen. Somewhat modified.
 */

#pragma once

#define CURRENT_CLASS Teensy3x

// https://www.pjrc.com/teensy/troubleshoot.html says:
// Teensy LC, 3.0, 3.1 do not have this problem.
// Variables defined with "const" are placed only in flash memory, but can be accessed normally.
// Only the older 8 bit AVR-based boards require these special steps to prevent strings
// and read-only variables from consuming limited RAM.
#define HAS_FLASH_READ      0
#define HAS_HARDWARE_PWM    1
#define HAS_SAMPLING        1
#define HAS_INPUT_CAPTURE   0

#define TIMER_INTR_NAME     cmt_isr
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#ifdef ISR
#undef ISR
#endif
#define  ISR(f)  void f(void)

#if F_BUS < 8000000
#error IRremote requires at least 8 MHz on Teensy 3.x
#endif

class Teensy3x : public Board {
public:

    Teensy3x() {
    };

    // not tested yet
    void reset() {
        // https://forum.pjrc.com/threads/52512-External-RESET-button-Teensy-3-2?p=180363&viewfull=1#post180363
        SCB_AIRCR = 0x05FA0004;
    }

private:

    void timerReset() {
        uint8_t tmp __attribute__((unused)) = CMT_MSC;
        CMT_CMD2 = 30U;
    };

    void timerEnablePwm() {
        CORE_PIN5_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_DSE | PORT_PCR_SRE;
    };

    void timerDisablePwm() {
        CORE_PIN5_CONFIG = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;
    };

    void timerEnableIntr() {
        NVIC_ENABLE_IRQ(IRQ_CMT);
    };

    void timerDisableIntr() {
        NVIC_DISABLE_IRQ(IRQ_CMT);
    };

    static const uint32_t CMT_PPS_DIV = (F_BUS + 7999999U) / 8000000U; // = 5

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        SIM_SCGC4 |= SIM_SCGC4_CMT;
        SIM_SOPT2 |= SIM_SOPT2_PTD7PAD;

        CMT_PPS = CMT_PPS_DIV - 1U;
        CMT_CGH1 = F_BUS / CMT_PPS_DIV / frequency * dutyCycle / 100U;
        CMT_CGL1 = F_BUS / CMT_PPS_DIV / frequency * (100 - dutyCycle) / 100U;
        CMT_CMD1 = 0U;
        CMT_CMD2 = 30U;
        CMT_CMD3 = 0U;
        CMT_CMD4 = 0U;
        CMT_OC = 0x60U;
        CMT_MSC = 0x01U;
    };

    void timerConfigNormal() {
        SIM_SCGC4 |= SIM_SCGC4_CMT;
        CMT_PPS = CMT_PPS_DIV - 1U;
        CMT_CGH1 = 1U;
        CMT_CGL1 = 1U;
        CMT_CMD1 = 0U;
        CMT_CMD2 = 30U;
        CMT_CMD3 = 0U;
        CMT_CMD4 = (F_BUS / 160000U + CMT_PPS_DIV / 2U) / CMT_PPS_DIV - 31U;
        CMT_OC = 0U;
        CMT_MSC = 0x03U;
    };

#define PWM_PIN  5
};