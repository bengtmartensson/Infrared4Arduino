#ifndef INFRAREDTYPES_H
#define	INFRAREDTYPES_H

#include <Arduino.h>

/**
 * Define some data types that should be used in the library.
 * Introduces some semantics.
 *
 * Do not add a size type!
 */

/**
 *  Type for durations in micro seconds. Change to a longer type if you need,
 *  AND know what you are doing.
 *  But DO NOT use a system dependent type like int!
 */
typedef uint16_t microseconds_t;
#define MICROSECONDS_T_MAX 65535
//typedef uint32_t microseconds_t;
//#define MICROSECONDS_T_MAX 4294967295

/**
 * Type for durations in milli seconds.
 * Using a larger type than 16 bits probably is not sensible.
 */
typedef uint16_t milliseconds_t;
#define MILLISECONDS_T_MAX 65535

/**
 * Type for modulation frequency in Hz.
 */
typedef uint16_t frequency_t;
#define FREQUENCY_T_MAX 65535

/**
 * Type for GPIO pin, compatible with Arduino libs.
 */
typedef uint8_t pin_t;
const pin_t invalidPin = 255;
#define PIN_T_MAX 255

#endif	// INFRAREDTYPES_H

