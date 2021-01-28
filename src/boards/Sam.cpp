// Support routines for SAM processor boards

// Largely based on https://forum.arduino.cc/index.php?topic=346731.0

// Partially based upon (heavily modified):
/*
  DimmerZero.cpp - Library for dimmer application with SAMD21G18A (e.g. Arduino Zero/M0).
  Created by E.Burkowski, February 18, 2017.
  Released into the public domain.
*/

/*
Recommended pwm pins: 2,3,4,5,6,7,11,13
For other pins see: https://github.com/Adminius/DimmerZero
Supported PWM frequencies 250Hz, 500Hz, 1000Hz. default is 1000Hz
*/

#if (defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)) & ! defined(ARDUINO_SAM_DUE)
#ifndef __SAMD21G18A__

#error "This library only supports SAMD21G18A based boards (e.g. Zero/M0...)"

#endif

#include "Board.h" // includes Sam.h

// following based on setup from GitHub jdneo/timerInterrupt.ino
void Sam::setTimerFrequency(frequency_t frequencyHz) {
    int compareValue = (F_CPU / (TIMER_PRESCALER_DIV * frequencyHz)) - 1;
    TcCount16* TC = (TcCount16*) TC3;
    // Make sure the count is in a proportional position to where it was
    // to prevent any jitter or disconnect when changing the compare value.
    TC->COUNT.reg = map(TC->COUNT.reg, 0, TC->CC[0].reg, 0, compareValue);
    TC->CC[0].reg = compareValue;
    while (TC->STATUS.bit.SYNCBUSY == 1);
}

void Sam::timerEnableIntr() {
    REG_GCLK_CLKCTRL = static_cast<uint16_t>(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3);
    while (GCLK->STATUS.bit.SYNCBUSY == 1); // wait for sync

    TcCount16* TC = (TcCount16*) TC3;

    TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

    // Use the 16-bit timer
    TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

    // Use match mode so that the timer counter resets when the count matches the compare register
    TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

    // Set prescaler to 1024
    //TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
    TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

    setTimerFrequency(1000000 / microsPerTick);

    // Enable the compare interrupt
    TC->INTENSET.reg = 0;
    TC->INTENSET.bit.MC0 = 1;

    NVIC_EnableIRQ(TC3_IRQn);

    TC->CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
}

void Sam::timerDisableIntr() {
    TcCount16* TC = (TcCount16*) TC3;
    NVIC_DisableIRQ(TC3_IRQn);
    TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
}


ISR(x); //interruptServiceRoutine(); // Defined in IRRemote as ISR(TIMER_INTR_NAME)

// Is used!!
void TC3_Handler() {
    TcCount16* TC = (TcCount16*) TC3;
    // If this interrupt is due to the compare register matching the timer count
    // we toggle the LED.
    if (TC->INTFLAG.bit.MC0 == 1) {
        TC->INTFLAG.bit.MC0 = 1;
        interruptServiceRoutine();
    }
}

void Sam::timerConfigHz(pin_t pin, frequency_t frequency, dutycycle_t dutyCycle) {
    pwmPin = pin;
    maxValue = F_CPU / 2 / frequency;
    onLength = maxValue * dutyCycle / 100;

    timerTCC0 = false;
    timerTCC1 = false;
    timerTCC2 = false;

    REG_GCLK_GENDIV = GCLK_GENDIV_DIV(1) |  // Divide the 48MHz clock source by divisor div: e.g. 48MHz/4=12MHz
            GCLK_GENDIV_ID(4);              // Select Generic Clock (GCLK) 4
    while (GCLK->STATUS.bit.SYNCBUSY);      // Wait for synchronization

    REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |   // Set the duty cycle to 50/50 HIGH/LOW
            GCLK_GENCTRL_GENEN |            // Enable GCLK4
            GCLK_GENCTRL_SRC_DFLL48M |      // Set the 48MHz clock source
            GCLK_GENCTRL_ID(4);             // Select GCLK4
    while (GCLK->STATUS.bit.SYNCBUSY);      // Wait for synchronization

    PORT->Group[g_APinDescription[pwmPin].ulPort].PINCFG[g_APinDescription[pwmPin].ulPin].bit.PMUXEN = 1;
    switch (pwmPin) {
        case 0:
            PORT->Group[g_APinDescription[1].ulPort].PMUX[g_APinDescription[1].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
            timerTCC1 = true;
            break;
        case 1:
            PORT->Group[g_APinDescription[1].ulPort].PMUX[g_APinDescription[1].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
            timerTCC1 = true;
            break;
        case 2:
            PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
            timerTCC0 = true;
            break;
        case 3:
            PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
            timerTCC1 = true;
            break;
        case 4:
            PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
            timerTCC1 = true;
            break;
        case 5:
            PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
            timerTCC0 = true;
            break;
        case 6:
            PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
            timerTCC0 = true;
            break;
        case 7:
            PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
            timerTCC0 = true;
            break;
        case 8:
            PORT->Group[g_APinDescription[8].ulPort].PMUX[g_APinDescription[8].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
            timerTCC1 = true;
            break;
        case 9:
            PORT->Group[g_APinDescription[8].ulPort].PMUX[g_APinDescription[8].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
            timerTCC1 = true;
            break;
        case 10:
            PORT->Group[g_APinDescription[10].ulPort].PMUX[g_APinDescription[10].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
            timerTCC0 = true;
            break;
        case 11:
            PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
            timerTCC2 = true;
            break;
        case 12:
            PORT->Group[g_APinDescription[10].ulPort].PMUX[g_APinDescription[10].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
            timerTCC0 = true;
            break;
        case 13:
            PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
            timerTCC2 = true;
            break;
        case A3:
            PORT->Group[g_APinDescription[A3].ulPort].PMUX[g_APinDescription[A3].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
            timerTCC0 = true;
            break;
        case A4:
            PORT->Group[g_APinDescription[A3].ulPort].PMUX[g_APinDescription[A3].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
            timerTCC0 = true;
            break;
        default:
            // error("Not supported pin!");
            break;
    }

    TCCx = timerTCC0 ? TCC0
            : timerTCC1 ? TCC1
            : TCC2;

    if (timerTCC0) {
        // Feed GCLK4 to TCC0
        REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |     // Enable GCLK4 to TCC0 and TCC1
                GCLK_CLKCTRL_GEN_GCLK4 |            // Select GCLK4
                GCLK_CLKCTRL_ID_TCC0_TCC1;          // Feed GCLK4 to TCC0 and TCC1
        while (GCLK->STATUS.bit.SYNCBUSY);          // Wait for synchronization

        if (invert) {
            REG_TCC0_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
            while (TCC0->SYNCBUSY.bit.WAVE);
        } else {
            REG_TCC0_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH; // Setup dual slope PWM on TCC0
            while (TCC0->SYNCBUSY.bit.WAVE);
        }

        REG_TCC0_PER = maxValue;                   // Set the frequency of the PWM on TCC0
        while (TCC0->SYNCBUSY.bit.PER);             // Wait for synchronization

        REG_TCC0_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 | // Divide GCLK4 by 1
                TCC_CTRLA_ENABLE;                   // Enable the TCC0 output
        while (TCC0->SYNCBUSY.bit.ENABLE);          // Wait for synchronization

    }
    if (timerTCC1) {
        // Feed GCLK4 to TCC1
        REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |     // Enable GCLK4 to TCC0 and TCC1
                GCLK_CLKCTRL_GEN_GCLK4 |            // Select GCLK4
                GCLK_CLKCTRL_ID_TCC0_TCC1;          // Feed GCLK4 to TCC0 and TCC1
        while (GCLK->STATUS.bit.SYNCBUSY);          // Wait for synchronization

        if (invert) {
            REG_TCC1_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
            while (TCC1->SYNCBUSY.bit.WAVE);
        } else {
            REG_TCC1_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH; // Setup dual slope PWM on TCC1
            while (TCC1->SYNCBUSY.bit.WAVE);
        }

        REG_TCC1_PER = maxValue;                   // Set the frequency of the PWM on TCC0
        while (TCC1->SYNCBUSY.bit.PER);             // Wait for synchronization

        REG_TCC1_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 | // Divide GCLK4 by 1
                TCC_CTRLA_ENABLE;                   // Enable the TCC1 output
        while (TCC1->SYNCBUSY.bit.ENABLE);          // Wait for synchronization

    }
    if (timerTCC2) {
        // Feed GCLK4 to TCC2
        REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |     // Enable GCLK4 to TCC0 and TCC1
                GCLK_CLKCTRL_GEN_GCLK4 |            // Select GCLK4
                GCLK_CLKCTRL_ID_TCC2_TC3;           // Feed GCLK4 to TCC0 and TCC1
        while (GCLK->STATUS.bit.SYNCBUSY);          // Wait for synchronization

        if (invert) {
            REG_TCC2_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
            while (TCC0->SYNCBUSY.bit.WAVE);
        } else {
            REG_TCC2_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH; // Setup dual slope PWM on TCC2
            while (TCC2->SYNCBUSY.bit.WAVE);
        }

        REG_TCC2_PER = maxValue;                   // Set the frequency of the PWM on TCC0
        while (TCC2->SYNCBUSY.bit.PER);             // Wait for synchronization

        REG_TCC2_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 | // Divide GCLK4 by 1
                TCC_CTRLA_ENABLE;                   // Enable the TCC2 output
        while (TCC2->SYNCBUSY.bit.ENABLE);          // Wait for synchronization

    }
}

void Sam::setValue(uint16_t value) {
    switch (pwmPin) {
        case 0:
            REG_TCC1_CC1 = value; // Set the PWM signal
            while (TCC1->SYNCBUSY.bit.CC1); // Wait for synchronization
            break;
        case 1:
            REG_TCC1_CC0 = value; // Set the PWM signal
            while (TCC1->SYNCBUSY.bit.CC0); // Wait for synchronization
            break;
        case 2:
            REG_TCC0_CC0 = value; // Set the PWM signal
            while (TCC0->SYNCBUSY.bit.CC0); // Wait for synchronization
            break;
        case 3:
            REG_TCC1_CC1 = value; // Set the PWM signal
            while (TCC1->SYNCBUSY.bit.CC1); // Wait for synchronization
            break;
        case 4:
            REG_TCC1_CC0 = value; // Set the PWM signal
            while (TCC1->SYNCBUSY.bit.CC0); // Wait for synchronization
            break;
        case 5:
            REG_TCC0_CC1 = value; // Set the PWM signal
            while (TCC0->SYNCBUSY.bit.CC1); // Wait for synchronization
            break;
        case 6:
            REG_TCC0_CC2 = value; // Set the PWM signal
            while (TCC0->SYNCBUSY.bit.CC2); // Wait for synchronization
            break;
        case 7:
            REG_TCC0_CC3 = value; // Set the PWM signal
            while (TCC0->SYNCBUSY.bit.CC3); // Wait for synchronization
            break;
        case 8:
            REG_TCC1_CC0 = value; // Set the PWM signal
            while (TCC1->SYNCBUSY.bit.CC0); // Wait for synchronization
            break;
        case 9:
            REG_TCC1_CC1 = value; // Set the PWM signal
            while (TCC1->SYNCBUSY.bit.CC1); // Wait for synchronization
            break;
        case 10:
            REG_TCC0_CC2 = value; // Set the PWM signal
            while (TCC0->SYNCBUSY.bit.CC2); // Wait for synchronization
            break;
        case 11:
            REG_TCC2_CC0 = value; // Set the PWM signal
            while (TCC2->SYNCBUSY.bit.CC0); // Wait for synchronization
            break;
        case 12:
            REG_TCC0_CC3 = value; // Set the PWM signal
            while (TCC0->SYNCBUSY.bit.CC3); // Wait for synchronization
            break;
        case 13:
            REG_TCC2_CC1 = value; // Set the PWM signal
            while (TCC2->SYNCBUSY.bit.CC1); // Wait for synchronization
            break;
        case A3:
            REG_TCC0_CC0 = value; // Set the PWM signal
            while (TCC0->SYNCBUSY.bit.CC0); // Wait for synchronization
            break;
        case A4:
            REG_TCC0_CC1 = value; // Set the PWM signal
            while (TCC0->SYNCBUSY.bit.CC1); // Wait for synchronization
            break;
        default:
            break;
    }
}

// https://forum.arduino.cc/index.php?topic=346731.120 post #121
//To stop the (TCC0) timer:
//
//Code: [Select]
//
//REG_TCC0_CTRLBSET = TCC_CTRLBSET_CMD_STOP;
//while(TCC0->SYNCBUSY.bit.CTRLB);
//
//
//To restart the timer again:
//
//Code: [Select]
//
//REG_TCC0_CTRLBSET = TCC_CTRLBSET_CMD_RETRIGGER;
//while(TCC0->SYNCBUSY.bit.CTRLB);

void Sam::timerConfigNormal() {
   TCCx->CTRLA.bit.ENABLE = 0;                     // Disable TCC1 timer
   while (TCCx->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
}

#endif // defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)