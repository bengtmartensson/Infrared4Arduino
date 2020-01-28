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

#include "boarddefs.h"

#ifdef USE_DEFAULT_ENABLE_IR_IN
#undef USE_DEFAULT_ENABLE_IR_IN
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define TIMER_SIZE 8
#define TIMER_RESET
#define TIMER_DISABLE_INTR /* empty */

#define SEND_PIN 5

#define TIMER_ENABLE_PWM        ledcWrite(LEDCHANNEL, 50)
#define TIMER_DISABLE_PWM       ledcWrite(LEDCHANNEL, 0)
#define TIMER_CONFIG_KHZ(khz)   do { \
    ledcSetup(LEDCHANNEL, khz*1000, TIMER_SIZE); \
    ledcAttachPin(SEND_PIN, LEDCHANNEL); \
} while (0)

#define TIMER_INTR_NAME
#define LEDCHANNEL 0

#ifdef ISR
#undef ISR
#endif
#define ISR(f)  void IrReceiverSampler::IRTimer()

class Esp32 {

};