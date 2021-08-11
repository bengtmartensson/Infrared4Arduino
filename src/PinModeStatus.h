/*
Copyright (C) 2020, 2021 Bengt Martensson.

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
 * @file PinModeStatus.h
 * @brief This file make sure that the types PinMode and PinStatus,
 * as well as the values HIGH and LOW are availabled.
 * To test this, we use the defined-ness of LOW.
 *
 * Some, but not all, architectures defines PinStatus and PinMode in a file like
 * ~/.arduino15/packages/arduino/hardware/.../cores/arduino/api/Common.h
 *
 * See also https://github.com/arduino/ArduinoCore-API/issues/25
 */

#pragma once

#if ! defined(ARDUINO)

typedef enum {
    LOW = 0,
    HIGH = 1,
    CHANGE = 2,
    FALLING = 3,
    RISING = 4,
} PinStatus;

typedef enum {
    INPUT = 0x0,
    OUTPUT = 0x1,
    INPUT_PULLUP = 0x2,
    INPUT_PULLDOWN = 0x3,
} PinMode;

#elif defined(LOW) || defined(DOXYGEN)// ARDUINO

/** Values of a digital pin. */
typedef int PinStatus;

/** Different operating modes of a GPIO pin. */
typedef int PinMode;

#else

// Is Arduino, but LOW is not defined.
// We assume that PinStatus and PinMode are defined as enums.

#endif
