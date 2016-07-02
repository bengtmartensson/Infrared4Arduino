#ifndef INFRAREDTYPES_H
#define	INFRAREDTYPES_H

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
#define MICROSECONDS_T_MAX 65535
//#define MICROSECONDS_T_MAX 4294967295

/**
 * Type for durations in milli seconds.
 * Using a larger type than 16 bits probably is not sensible.
 */
typedef uint16_t milliseconds_t;
/** Largest milliseconds_t number possible */
#define MILLISECONDS_T_MAX 65535

/**
 * Type for modulation frequency in Hz.
 */
typedef uint16_t frequency_t;
/** Largest frequency_t number possible */
#define FREQUENCY_T_MAX 65535

/**
 * Type for GPIO pin, compatible with Arduino libs.
 */
typedef uint8_t pin_t;
/** Symbolic name for an invalid pin number */
const pin_t invalidPin = 255;
/** Largest pin_t number possible */
#define PIN_T_MAX 255

#endif	// INFRAREDTYPES_H

