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
 * This file defines the enums PinStatus and PinMode unless already known.
 * To test this, we use the defined-ness of LOW.
 */

#pragma once

// Some, but not all, architectures defines PinStatus and PinMode in a file like
// ~/.arduino15/packages/arduino/hardware/*/*/cores/arduino/api/Common.h
// I make the guess that if and only if LOW is defined as macro,
// the definition should be made.

// See also https://github.com/arduino/ArduinoCore-API/issues/25

#if defined(LOW) || ! defined(ARDUINO)

#ifdef LOW
#undef LOW
#endif

#ifdef HIGH
#undef HIGH
#endif

#ifdef CHANGE
#undef CHANGE
#endif

#ifdef FALLING
#undef FALLING
#endif

#ifdef RISING
#undef RISING
#endif

typedef enum {
    LOW = 0,
    HIGH = 1,
    CHANGE = 2,
    FALLING = 3,
    RISING = 4,
} PinStatus;

#ifdef INPUT
#undef INPUT
#endif

#ifdef OUTPUT
#undef OUTPUT
#endif

#ifdef INPUT_PULLUP
#undef INPUT_PULLUP
#endif

#ifdef INPUT_PULLDOWN
#undef INPUT_PULLDOWN
#endif

typedef enum {
    INPUT = 0x0,
    OUTPUT = 0x1,
    INPUT_PULLUP = 0x2,
    INPUT_PULLDOWN = 0x3,
} PinMode;

#endif
