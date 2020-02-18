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

#ifdef USE_DEFAULT_ENABLE_IR_IN
#undef USE_DEFAULT_ENABLE_IR_IN
#endif

class Sam {
#define TIMER_RESET
#define TIMER_ENABLE_PWM     Sam::enable_pwm()
#define TIMER_DISABLE_PWM    Sam::disable_pwm()
//#define TIMER_ENABLE_INTR    NVIC_EnableIRQ(TC3_IRQn) // Not presently used
#define TIMER_DISABLE_INTR   NVIC_DisableIRQ(TC3_IRQn)
//#define TIMER_INTR_NAME      TC3_Handler // Not presently used
#define TIMER_CONFIG_KHZ(f)  Sam::pwm_init(f)

#ifdef ISR
#undef ISR
#endif
#define ISR(f)  void interruptServiceRoutine()

#define SEND_PIN 12 // 3

    static const unsigned int timer_prescale = 1U;

    static const unsigned int TIMER_PRESCALER_DIV = 64U;

    static void setTimerFrequency(frequency_t hz);

public:
    static void startTimer();

    static void enable_pwm();

    static void disable_pwm();

    static void pwm_init(frequency_t f);
};
