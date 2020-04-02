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
 * TODO
 */

#pragma once

#if !defined(ARDUINO) || defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_SAMD) \
 || defined(ARDUINO_ARCH_SAM) || defined(TEENSYDUINO) || defined(ARDUINO_ARCH_ESP32) \
 || defined(ARDUINO_ARCH_ESP8266)

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
