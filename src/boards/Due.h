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

// This is basically copied from
// https://github.com/enternoescape/Arduino-IRremote-Due
// (License: LGPL 2.1)
// with minor adjustments.

#pragma once

#define CURRENT_CLASS Due

#define HAS_FLASH_READ      1
#define HAS_HARDWARE_PWM    1
#define HAS_SAMPLING        1
#define HAS_INPUT_CAPTURE   0

#define STRCPY_PF_CAST(x) static_cast<const char*>(x)

class Due : public Board {
public:
    Due() {
    };

private:

#if defined(IR_USE_PWM0) // pin 34 ////////////////////////////////////////////
#define PWM_PIN 34
#define IR_USE_PWM_PORT PIOC
#define IR_USE_PWM_PERIPH PIO_PERIPH_B
#define IR_USE_PWM_PINMASK PIO_PC2
#define IR_USE_PWM_CH 0
#elif defined(IR_USE_PWM1) // pin 36 //////////////////////////////////////////
#define PWM_PIN 36
#define IR_USE_PWM_PORT PIOC
#define IR_USE_PWM_PERIPH PIO_PERIPH_B
#define IR_USE_PWM_PINMASK PIO_PC4
#define IR_USE_PWM_CH 1
#elif defined(IR_USE_PWM2) // pin 38 //////////////////////////////////////////
#define PWM_PIN 38
#define IR_USE_PWM_PORT PIOC
#define IR_USE_PWM_PERIPH PIO_PERIPH_B
#define IR_USE_PWM_PINMASK PIO_PC6
#define IR_USE_PWM_CH 2
#elif defined(IR_USE_PWM3) // pin 40 //////////////////////////////////////////
#define PWM_PIN 40
#define IR_USE_PWM_PORT PIOC
#define IR_USE_PWM_PERIPH PIO_PERIPH_B
#define IR_USE_PWM_PINMASK PIO_PC8
#define IR_USE_PWM_CH 3
#elif defined(IR_USE_PWM4) // pin 9 ///////////////////////////////////////////
#define PWM_PIN 9
#define IR_USE_PWM_PORT PIOC
#define IR_USE_PWM_PERIPH PIO_PERIPH_B
#define IR_USE_PWM_PINMASK PIO_PC21
#define IR_USE_PWM_CH 4
#elif defined(IR_USE_PWM5) // pin 8 ///////////////////////////////////////////
#define PWM_PIN 8
#define IR_USE_PWM_PORT PIOC
#define IR_USE_PWM_PERIPH PIO_PERIPH_B
#define IR_USE_PWM_PINMASK PIO_PC22
#define IR_USE_PWM_CH 5
#elif defined(IR_USE_PWM6) // pin 7 ///////////////////////////////////////////
#define PWM_PIN 7
#define IR_USE_PWM_PORT PIOC
#define IR_USE_PWM_PERIPH PIO_PERIPH_B
#define IR_USE_PWM_PINMASK PIO_PC23
#define IR_USE_PWM_CH 6
#elif defined(IR_USE_PWM7) // pin 6 ///////////////////////////////////////////
#define PWM_PIN 6
#define IR_USE_PWM_PORT PIOC
#define IR_USE_PWM_PERIPH PIO_PERIPH_B
#define IR_USE_PWM_PINMASK PIO_PC24
#define IR_USE_PWM_CH 7
#endif

#if defined(IR_USE_TC3) // Timer clock 3 //////////////////////////////////////
#define IR_USE_TC_IRQ  TC3_IRQn
#define IR_USE_TC   TC1
#define IR_USE_CH   0
#define TIMER_INTR_NAME TC3_Handler
#elif defined(IR_USE_TC4) // Timer clock 4 ////////////////////////////////////
#define IR_USE_TC_IRQ  TC4_IRQn
#define IR_USE_TC   TC1
#define IR_USE_CH   1
#define TIMER_INTR_NAME TC4_Handler
#elif defined(IR_USE_TC5) // Timer clock 5 ////////////////////////////////////
#define IR_USE_TC_IRQ  TC5_IRQn
#define IR_USE_TC   TC1
#define IR_USE_CH   2
#define TIMER_INTR_NAME TC5_Handler
#endif

    //Clears the interrupt.
    void timerReset() {
        IR_USE_TC->TC_CHANNEL[IR_USE_CH].TC_SR;
    }

    void timerEnablePwm() {
        PWMC_EnableChannel(PWM_INTERFACE, IR_USE_PWM_CH);
    };

    void timerDisablePwm() {
        PWMC_DisableChannel(PWM_INTERFACE, IR_USE_PWM_CH);
        writeLow();
    };

    void timerEnableIntr() {
        NVIC_EnableIRQ(IR_USE_TC_IRQ);
    };

    void timerDisableIntr() {
        NVIC_DisableIRQ(IR_USE_TC_IRQ);
    };

    static const unsigned multiplicator = 20U; // was 2U;

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        pmc_enable_periph_clk(PWM_INTERFACE_ID);
        const uint32_t pwmval = frequency * multiplicator;
        PWMC_ConfigureClocks(PWM_FREQUENCY * PWM_MAX_DUTY_CYCLE, pwmval, F_CPU);
        PIO_Configure(IR_USE_PWM_PORT, IR_USE_PWM_PERIPH, IR_USE_PWM_PINMASK, PIO_DEFAULT);
        PWMC_ConfigureChannel(PWM_INTERFACE, IR_USE_PWM_CH, PWM_CMR_CPRE_CLKB, 0U, 0U);
        PWMC_SetPeriod(PWM_INTERFACE, IR_USE_PWM_CH, multiplicator);
        PWMC_SetDutyCycle(PWM_INTERFACE, IR_USE_PWM_CH, static_cast<uint16_t>((multiplicator * dutyCycle + 50U) / 100U));
    };

    void timerConfigNormal() {
        pmc_enable_periph_clk(static_cast<uint32_t>(IR_USE_TC_IRQ));
        TC_Configure(IR_USE_TC, IR_USE_CH, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1);
        const uint32_t rc = F_CPU / 2U * microsPerTick / 1000000U;
        TC_SetRA(IR_USE_TC, IR_USE_CH, rc / 2U);
        TC_SetRC(IR_USE_TC, IR_USE_CH, rc);
        TC_Start(IR_USE_TC, IR_USE_CH);
        IR_USE_TC->TC_CHANNEL[IR_USE_CH].TC_IER = TC_IER_CPCS;
        IR_USE_TC->TC_CHANNEL[IR_USE_CH].TC_IDR = ~TC_IER_CPCS;
    };
};