#pragma once

#include "Board.h"
// Configuration options for MicroGirs.

// Define Girs modules to implement, see http://www.harctoolbox.org/Girs.html

// Transmit IR signals, requires IR Led(s).
#define TRANSMIT

// Support sending signals in Pronto Hex format.
#define PRONTO

// Render some known protocols, presently NEC1 and RC5
#define RENDERER

// Capture IR signals, requires non-demodulating IR sensor (TSMP58000, QSE159, etc)
#if HAS_INPUT_CAPTURE
#define CAPTURE
#endif

// Receive demodulated IR signals, require demodulating IR sensor (TSMP* or equivalent)
#define RECEIVE

// Allow to change the parameters during runtime, like timeouts
#define PARAMETERS

// Support sending signals without modulation, e.g. with RF module.
#define NON_MOD

// Character that ends the command lines
#define EOLCHAR '\r'

// Without PARAMETERS, these are really not defaults,
// they are the non-changeable values.
#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds
#define DEFAULT_CAPTURE_ENDINGTIMEOUT 100L // milliseconds
#define DEFAULT_RECEIVE_ENDINGTIMEOUT 50L // milliseconds

// Size of capture and receive arrays
#define DEFAULT_CAPTURESIZE 400U // must be even

#ifdef RECEIVE
// This quantity is added to all gaps and subtracted from all marks when receiving.
#define IRRECEIVER_MARK_EXCESS 50
#endif

#ifdef CAPTURE
// This quantity is added to all gaps and subtracted from all marks when capturing.
#define IRSENSOR_MARK_EXCESS -10
#endif

// Baud rate for serial interface
#define BAUD 115200
#define SERIAL_TIMEOUT 5000L

// Hardware configuration

// Use internal pullup for the capture (non-demodulating) sensor?
#define IRCAPTURER_PULLUP false

// Pin where the receiving (demodulating) sensor is connected.
#ifdef ESP32
#define IRRECEIVER_PIN      4U
#elif defined(ESP8266)
#define IRRECEIVER_PIN      2U
#elif defined(ARDUINO_AVR_MICRO)
#define IRRECEIVER_PIN      10
#elif defined (ARDUINO_TEENSY32) // Teenex 3.1-3.2
#define IRRECEIVER_PIN      7U
#else
#define IRRECEIVER_PIN      5U
#endif

// Use internal pullup for the capture (non-demodulating) sensor?
#define IRRECEIVER_PULLUP false

#ifdef ARDUINO_AVR_NANO
#define IRRECEIVER_1_GND    6U
#define IRRECEIVER_1_VCC    7U
#define IRSENSOR_1_GND      9U
#define IRSENSOR_1_VCC      10U
#endif

#ifdef ARDUINO_AVR_MICRO
#define IRRECEIVER_1_GND    16U
#define IRRECEIVER_1_VCC    14U
#define IRSENSOR_1_GND      5U
#define IRSENSOR_1_VCC      6U
#endif

// Pin where the sender for non-modulated signals is connected, e.g. 434MHz RF.
#ifdef NON_MOD
#define NON_MOD_PIN 10
#endif
