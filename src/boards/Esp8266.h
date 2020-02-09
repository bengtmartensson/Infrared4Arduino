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
 * @file esp8266.h
 *
 * Board definitions for the Espressif ESP8666.
 * This board comes without explicitly supported hardware PWM.
 * (For a hack, implementing PWM by misusing the UARRT, see
 * https://www.analysir.com/blog/2017/01/29/updated-esp8266-nodemcu-backdoor-upwm-hack-for-ir-signals/
 */

#pragma once

#ifdef HAS_FLASH_READ
#undef HAS_FLASH_READ
#endif

#undef HAS_HARDWARE_PWM

#ifdef USE_DEFAULT_ENABLE_IR_IN
#undef USE_DEFAULT_ENABLE_IR_IN
#endif

// Supply own enbleIRIn
#undef USE_DEFAULT_ENABLE_IR_IN

#define USE_US_TIMER 1

extern "C" {
#include "user_interface.h"
}

// The default pin used used for sending.
//#define SEND_PIN 4 // pin 4 = D2 on board

//#define TIMER_CONFIG_KHZ(x) Current board does not support hardware PWM. Consider using IrSenderSoftPwm or IrSenderSpin instead.

//#define TIMER_CONFIG_NORMAL() /* nothing */
//#define TIMER_ENABLE_INTR
#if 1
#define TIMER_DISABLE_INTR do { \
        os_timer_disarm(&Esp8266::timer); \
} while(false)
#else
#define TIMER_DISABLE_INTR
#endif
#define TIMER_RESET

#ifdef ISR
#undef ISR
#endif
#define ISR(f) void ICACHE_RAM_ATTR IRTimer(void *pArg __attribute__((unused)))

void IRTimer(void *pArg); // defined in IrReceiverSampler.cpp, masqueraded as ISR(TIMER_INTR_NAME)

class Esp8266 {
public:
    static os_timer_t timer;
};