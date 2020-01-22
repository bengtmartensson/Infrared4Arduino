#pragma once

/**
 * @file boarddefs.h
 *
 * @brief All board specific information should be contained in this file.
 * It defines a number of macros, depending on the board, as determined by
 * pre-processor symbols.
 */

// IRremote
// Version 2.0.1 June, 2015
// Copyright 2009 Ken Shirriff
// For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html

// Modified by Paul Stoffregen <paul@pjrc.com> to support other boards and timers
//
// Interrupt code based on NECIRrcv by Joe Knapp
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
// Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/

//******************************************************************************

// Define some defaults, that some boards may like to override
// (This is to avoid negative logic, ! DONT_... is just awkward.)

#ifdef DOXYGEN
///**
// * Define if the current board has/needs the header avr/interrupt.h.
// */
//#define HAS_AVR_INTERRUPT_H

///**
// * Define if the current board supports sending.
// */
//#define SENDING_SUPPORTED

/**
 * Defined if the standard enableIRIn function should be used.
 * Undefine for boards supplying their own.
 */
#define USE_DEFAULT_ENABLE_IR_IN

///**
// * Duty cycle in percent for sent signals.
// * Presently takes effect only together with USE_SOFT_CARRIER.
// */
//#define DUTY_CYCLE 40

/**
 * If USE_SOFT_CARRIER, this amount (in micro seconds) is subtracted from the
 * on-time of the pulses.
 */
//#define PULSE_CORRECTION 3

//------------------------------------------------------------------------------
// This first #ifdef statement contains
// all other board specific information, with the exception of
// times and the sending pin (SEND_PIN).

/**
 * Define to use carrier generation in software, instead of hardware PWM.
 */
//#define USE_SOFT_CARRIER

/**
 * Define to use spin wait instead of delayMicros() for USE_SOFT_CARRIER.
 */
//#define USE_SPIN_WAIT

///**
// * Arduino CPU frequency
// */
//#define SYSCLOCK  F_CPU
#endif // DOXYGEN

///////////////////////////////////////////////////////////////////////////////
#if 0
//------------------------------------------------------------------------------
// Defines for blinking the LED
//

#if defined(CORE_LED0_PIN)
#	define BLINKLED        CORE_LED0_PIN
#	define BLINKLED_ON()   (digitalWrite(CORE_LED0_PIN, HIGH))
#	define BLINKLED_OFF()  (digitalWrite(CORE_LED0_PIN, LOW))

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#	define BLINKLED        13
#	define BLINKLED_ON()   (PORTB |= B10000000)
#	define BLINKLED_OFF()  (PORTB &= B01111111)

#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
#	define BLINKLED        0
#	define BLINKLED_ON()   (PORTD |= B00000001)
#	define BLINKLED_OFF()  (PORTD &= B11111110)

#elif defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)
#	define BLINKLED        LED_BUILTIN
#	define BLINKLED_ON()   (digitalWrite(LED_BUILTIN, HIGH))
#	define BLINKLED_OFF()  (digitalWrite(LED_BUILTIN, LOW))

#	define USE_SOFT_CARRIER
	// Define to use spin wait instead of delayMicros()
#	define USE_SPIN_WAIT
#       undef USE_DEFAULT_ENABLE_IR_IN

        // The default pin used used for sending.
#	define SEND_PIN 9

#elif defined(ESP32)
        // No system LED on ESP32, disable blinking by NOT defining BLINKLED

        // avr/interrupt.h is not present
#       undef HAS_AVR_INTERRUPT_H

        // Sending not implemented
#       undef SENDING_SUPPORTED#

        // Supply own enbleIRIn
#       undef USE_DEFAULT_ENABLE_IR_IN

#elif defined(ESP8266)

// avr/interrupt.h is not present
#       undef HAS_AVR_INTERRUPT_H

#	define USE_SOFT_CARRIER

	// Supply own enbleIRIn
#       undef USE_DEFAULT_ENABLE_IR_IN

	// The default pin used used for sending.
#	define SEND_PIN 4 // pin 4 = D2 on board

#else
#	define BLINKLED        13
#	define BLINKLED_ON()  (PORTB |= B00100000)
#	define BLINKLED_OFF()  (PORTB &= B11011111)
#endif
#endif
//------------------------------------------------------------------------------
// CPU Frequency
//
#ifndef F_CPU
#error F_CPU not defined. This must be fixed.
#endif

/**
 * CPU frequency
 */
//#define SYSCLOCK  F_CPU

/**
 * microseconds per clock interrupt tick
 */
#define USECPERTICK    50

//------------------------------------------------------------------------------
// Define which timer to use
//
// Uncomment the timer you wish to use on your board.
// If you are using another library which uses timer2, you have options to
// switch IRremote to use a different timer.
//

#if ! defined(ARDUINO)

// Assume that we compile a test version, to be executed on the host, not on a board.
#include "boards/sil.h"

#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)

/////////////////// Arduino Uno, Nano etc (previously default clause)
// Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, Nano, etc
// ATmega48, ATmega88, ATmega168, ATmega328
//#define IR_USE_TIMER1   // tx = pin 9
#define IR_USE_TIMER2     // tx = pin 3

#include "boards/ATmega328P.h"

#elif defined(__AVR_ATmega2560__)
// defined(__AVR_ATmega1280__) ||

/////////////////// Mega 2560 etc //////////////////////////////////
//#define IR_USE_TIMER1   // tx = pin 11
#define IR_USE_TIMER2     // tx = pin 9
//#define IR_USE_TIMER3   // tx = pin 5
//#define IR_USE_TIMER4   // tx = pin 6
//#define IR_USE_TIMER5   // tx = pin 46

#include "boards/ATmega2560.h"

//#elif defined(__AVR_AT90USB162__)
//
//// Teensy 1.0
//#define IR_USE_TIMER1     // tx = pin 17
//
//#include "boards/teensy10.h"

#elif defined(__AVR_ATmega32U4__)

// Can be either Leonardo, Micro, or Teensy 2.
#if defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO)
#define IR_USE_TIMER1     // tx = 9
//#define IR_USE_TIMER2   // tx = 3, currently broken

#include "boards/ATmega32U4.h"

#else
//// Teensy 2.0
////#define IR_USE_TIMER1   // tx = pin 14
////#define IR_USE_TIMER3   // tx = pin 9
//#define IR_USE_TIMER4_HS  // tx = pin 10
//
//#include "boards/teensy20.h"

#error Unsupported board. Please fix boarddefs.h.

#endif

// Teensy 3.0 / Teensy 3.1
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
//#define IR_USE_TIMER_CMT  // tx = pin 5

#include "boards/teensy3x.h"

// Teensy-LC
//#elif defined(__MKL26Z64__)
//#define IR_USE_TIMER_TPM1 // tx = pin 16
//
//#include "boards/teensyLC.h"
//
//// Teensy++ 1.0 & 2.0
//#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
//
////#define IR_USE_TIMER1   // tx = pin 25
//#define IR_USE_TIMER2     // tx = pin 1
////#define IR_USE_TIMER3   // tx = pin 16
//
//#include "boards/teensy++.h"
//
//// MightyCore - ATmega1284
//#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
////#define IR_USE_TIMER1   // tx = pin 13
//#define IR_USE_TIMER2     // tx = pin 14
////#define IR_USE_TIMER3   // tx = pin 6
//
//#include "boards/mightycore_1284.h"
//
//// MightyCore - ATmega164, ATmega324, ATmega644
/* /#elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) \
//|| defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324A__) \
//|| defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega164A__) \
//|| defined(__AVR_ATmega164P__) */
////#define IR_USE_TIMER1   // tx = pin 13
//#define IR_USE_TIMER2     // tx = pin 14
//
//#include "boards/mightycore.h"
//
////MegaCore - ATmega64, ATmega128
//#elif defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
// #define IR_USE_TIMER1     // tx = pin 13
//
//#include "boards/mightycore_64.h"
//
//// MightyCore - ATmega8535, ATmega16, ATmega32
//#elif defined(__AVR_ATmega8535__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
// #define IR_USE_TIMER1     // tx = pin 13
//
//#include "boards/mightcore_32.h"
//
//#elif defined(__AVR_ATmega8__)
//// Atmega8
//#define IR_USE_TIMER1     // tx = pin 9
//#include "boards/atmega8.h"
//
//// ATtiny84
//#elif defined(__AVR_ATtiny84__)
//#define IR_USE_TIMER1     // tx = pin 6
//
//#include "boards/attiny84.h"
//
////ATtiny85
//#elif defined(__AVR_ATtiny85__)
//#define IR_USE_TIMER_TINY0   // tx = pin 1
//#include "boards/attiny84.h"
//
//
#elif defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)
#define TIMER_PRESCALER_DIV 64

//#define USE_SOFT_CARRIER
// Define to use spin wait instead of delayMicros()
//#define USE_SPIN_WAIT
#undef USE_DEFAULT_ENABLE_IR_IN

#include "boards/sam32.h"

///////////////////// ESP32
#elif defined(ESP32)
#define IR_TIMER_USE_ESP32
// avr/interrupt.h is not present
#undef HAS_AVR_INTERRUPT_H

// Sending not implemented
#undef SENDING_SUPPORTED

// Supply own enbleIRIn
#undef USE_DEFAULT_ENABLE_IR_IN
#include "boards/esp32.h"

///////////////////// ESP8266
#elif defined(ESP8266)

// avr/interrupt.h is not present
#undef HAS_AVR_INTERRUPT_H

#define USE_SOFT_CARRIER

// Supply own enbleIRIn
#undef USE_DEFAULT_ENABLE_IR_IN

// The default pin used used for sending.
#define SEND_PIN 4 // pin 4 = D2 on board
#include "boards/esp8266.h"
#else

#error Unsupported board. Please fix boarddefs.h.

#endif

// Provide default definitions, portable but possibly slower than necessary.
// digitalWrite is supposed to be slow. If this is an issue, define faster,
// board-dependent versions of these macros SENDPIN_ON(pin) and SENDPIN_OFF(pin).
// Portable, possibly slow, default definitions are given at the end of this file.
// If defining new versions, feel free to ignore the pin argument if it
// is not configurable on the current board.

#ifndef SENDPIN_ON
/** Board dependent macro to turn on the pin given as argument. */
#define SENDPIN_ON(pin)  digitalWrite(pin, HIGH)
#endif

#ifndef SENDPIN_OFF
/**
 * Board dependent macro to turn off the pin given as argument.
 */
#define SENDPIN_OFF(pin) digitalWrite(pin, LOW)
#endif
