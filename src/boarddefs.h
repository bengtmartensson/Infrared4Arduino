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

static const uint16_t dutyCyclePercent = 40;

// Default value

#define HAS_HARDWARE_PWM

#if ! defined(ARDUINO)

// Assume that we compile a test version, to be executed on the host, not on a board.
#include "boards/sil.h"

#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
/////////////////// Arduino Uno, Nano etc (previously default clause)
//#define IR_USE_TIMER1   // tx = pin 9
#define IR_USE_TIMER2     // tx = pin 3

#include "boards/ATmega328P.h"

#elif defined(__AVR_ATmega2560__)
/////////////////// Mega 2560 etc //////////////////////////////////
//#define IR_USE_TIMER1   // tx = pin 11
#define IR_USE_TIMER2     // tx = pin 9
//#define IR_USE_TIMER3   // tx = pin 5
//#define IR_USE_TIMER4   // tx = pin 6
//#define IR_USE_TIMER5   // tx = pin 46

#include "boards/ATmega2560.h"

#elif defined(__AVR_ATmega32U4__)
// Can be either Leonardo, Micro, or Teensy 2.
#if defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO)
#define IR_USE_TIMER1     // tx = 9
//#define IR_USE_TIMER2   // tx = 3, currently broken

#include "boards/ATmega32U4.h"

#else
//// Teensy 2.0 ?
#error Unsupported board. Please fix boarddefs.h.
#endif

#elif defined(__AVR_ATmega4809__)
// ATMega4809, like Uno WiFi Rev 2, Nano Every
#define IR_USE_TIMER1     //  tx = pin 6
//#define IR_USE_TIMER2     // Not yet implemented tx = ?

#include "boards/ATmega4809.h"

#elif defined(__SAM3X8E__) || defined(__SAM3X8H__)
// Arduino Due

  //#define IR_USE_PWM0   // tx = pin 34
  //#define IR_USE_PWM1   // tx = pin 36
  //#define IR_USE_PWM2   // tx = pin 38
  //#define IR_USE_PWM3   // tx = pin 40
  //#define IR_USE_PWM4   // tx = pin 9
  //#define IR_USE_PWM5   // tx = pin 8
  #define IR_USE_PWM6   // tx = pin 7
  //#define IR_USE_PWM7   // tx = pin 6

  #define IR_USE_TC3    // Use timer clock 3.
  //#define IR_USE_TC4    // Use timer clock 4.
  //#define IR_USE_TC5    // Use timer clock 5.

//  #define IR_USE_SAM // Used to correct code where needed to be compatible with the Due.
//  #define IR_USE_DUE // Used to correctly map pins. (The idea being there might be more than one Arduino model based on SAM cores.)

#include "boards/due.h"

#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
// Teensy 3.0 / Teensy 3.1 / 3.2

#include "boards/teensy3x.h"

#elif defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)

#include "boards/Sam.h"

///////////////////// ESP32
#elif defined(ESP32)

#include "boards/Esp32.h"

///////////////////// ESP8266
#elif defined(ESP8266)

#include "boards/Esp8266.h"

#else

#error Your board is currently not supported. Please add it to boarddefs.h.

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
