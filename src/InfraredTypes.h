#pragma once

#include <Arduino.h>

/**
 * @file InfraredTypes.h
 * @brief This file defines some general data types that are used in the library.
 */

/**
 * Type for durations in micro seconds. Change to a longer type if needed,
 * AND you know what you are doing.
 * But DO NOT use a system dependent type like int!
 */
typedef uint16_t microseconds_t;
//typedef uint32_t microseconds_t;
/** Largest microseconds_t number possible */
static constexpr microseconds_t MICROSECONDS_T_MAX = static_cast<microseconds_t>(-1);

/**
 * Type for durations in milli seconds.
 * Using a larger type than 16 bits probably is not sensible.
 */
typedef uint16_t milliseconds_t;
/** Largest milliseconds_t number possible */
static constexpr milliseconds_t MILLISECONDS_T_MAX = static_cast<milliseconds_t>(-1);

/**
 * Type for modulation frequency in Hz.
 */
typedef uint32_t frequency_t;

/**
 * Type for duty cycle in percent. Less than 0 means no information/don't care.
 */
typedef int8_t dutycycle_t;

/**
 * Type for GPIO pin, compatible with Arduino libs.
 */
typedef uint8_t pin_t;
/** Symbolic name for an invalid pin number */
static constexpr pin_t invalidPin = 255;
/** Largest pin_t number possible */
static constexpr pin_t PIN_T_MAX = 255;
