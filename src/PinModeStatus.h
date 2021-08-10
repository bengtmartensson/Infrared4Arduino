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
#if CHANGE == 0x03
#define _ESP_PIN_STATUS_STYLE
#endif
#undef CHANGE
#endif

#ifdef FALLING
#undef FALLING
#endif

#ifdef RISING
#undef RISING
#endif

#ifdef _ESP_PIN_STATUS_STYLE
#undef _ESP_PIN_STATUS_STYLE
// https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-gpio.h
// https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Arduino.h
typedef enum {
    LOW = 0,
    HIGH = 1,
    CHANGE = 3,
    FALLING = 2,
    RISING = 1,
} PinStatus;

#else
typedef enum {
    LOW = 0,
    HIGH = 1,
    CHANGE = 2,
    FALLING = 3,
    RISING = 4,
} PinStatus;
#endif

#ifdef INPUT
#undef INPUT
#endif

#ifdef OUTPUT
#if OUTPUT == 0x02
#define _ESP32_PIN_STYLE
#endif
#undef OUTPUT
#endif

#ifdef INPUT_PULLUP
#undef INPUT_PULLUP
#endif

#ifdef INPUT_PULLDOWN
#undef INPUT_PULLDOWN
#endif

#ifdef _ESP32_PIN_STYLE
#undef _ESP32_PIN_STYLE
// https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-gpio.h
typedef enum {
    INPUT = 0x01,
    OUTPUT = 0x02,
    INPUT_PULLUP = 0x05,
    INPUT_PULLDOWN = 0x09,
} PinMode;
#else
typedef enum {
    INPUT = 0x0,
    OUTPUT = 0x1,
    INPUT_PULLUP = 0x2,
    INPUT_PULLDOWN = 0x3,
} PinMode;
#endif

#endif
