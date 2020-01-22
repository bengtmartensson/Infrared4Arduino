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

/**
 * @file sil.h
 *
 * @brief This file contains "hardware" definition for generating
 * SIL simulation code.
 */


#ifdef ARDUINO
#error This file is not for compiling for the Arduino.
#endif


//#undef HAS_AVR_INTERRUPT_H
#define TIMER_DISABLE_INTR /* as nothing */
#define SEND_PIN 255
#define TIMER_DISABLE_PWM /* as nothing */
#define TIMER_ENABLE_PWM /* as nothing */
#define TIMER_CONFIG_KHZ(x) /* as nothing */
