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

#pragma once

#include "InfraredTypes.h"

#define CURRENT_CLASS Sam

#define HAS_FLASH_READ      1
#define HAS_HARDWARE_PWM    1
#define HAS_SAMPLING        1
#define HAS_INPUT_CAPTURE   0

#define STRCPY_PF_CAST(x) static_cast<const char *>(x)

//  Default PWM pin
#define PWM_PIN 3

#ifdef ISR
#undef ISR
#endif
#define ISR(f)  void interruptServiceRoutine()

class Sam : public Board {
public:

    Sam() {
    };

private:
    pin_t pwmPin;
    static constexpr bool invert = false;
    uint16_t maxValue;
    uint16_t onLength;
    bool timerTCC0;
    bool timerTCC1;
    bool timerTCC2;
    Tcc* TCCx;

    void timerConfigNormal();

    void timerEnableIntr();

    void timerDisableIntr();

    void timerConfigHz(frequency_t hz __attribute__ ((unused)), dutycycle_t dutyCycle __attribute__ ((unused))) {
        timerConfigHz(PWM_PIN, hz, dutyCycle);
    };

    void timerConfigHz(pin_t pin, frequency_t hz, dutycycle_t dutyCycle);

    void timerEnablePwm() {
        setValue(onLength);
    };

    void timerDisablePwm() {
        setValue(0U);
    }

    static const unsigned int TIMER_PRESCALER_DIV = 64U;

    void setTimerFrequency(frequency_t hz);

    void setValue(uint16_t value);
};
