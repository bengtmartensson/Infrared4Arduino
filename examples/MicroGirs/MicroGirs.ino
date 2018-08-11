/*
Copyright (C) 2014,2015,2017,2018 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

/*
Implements a simple Girs server for the Arduino.
It can be used with IrScrutinizer Lirc (using the [Girs driver].
It is an interactive server that can send and receive IR signals,
with some bells and whistles.
The interactive server is mainly meant for interact with other programs.
communicating over a serial line, likely in USB disguise.
*/

// All configurations are found in this file.
#include "config.h"

// Same version as Infrared4Arduino.
#define VERSION "1.0.3"

#ifdef RECEIVE
#include <IrReceiverSampler.h>
#endif

#ifdef TRANSMIT
#include <IrSenderPwm.h>
#endif

#ifdef RECEIVE
#include <IrReceiverSampler.h>
#endif

#ifdef CAPTURE
#include <IrWidgetAggregating.h>
#endif

#ifdef NON_MOD
#include <IrSenderNonMod.h>
#endif

#ifdef PRONTO
#include <Pronto.h>
#endif

#ifdef RENDERER
#ifndef TRANSMIT
#error RENDER without TRANSMIT is nonsensical, aborting.
#endif
#include <IrSignal.h>
#include <Nec1Renderer.h>
#include <Rc5Renderer.h>
#endif

#include "Tokenizer.h"

#ifdef TRANSMIT
#define TRANSMIT_NAME Transmit
#else
#define TRANSMIT_NAME
#endif

#ifdef PRONTO
#define PRONTO_NAME Pronto
#else
#define PRONTO_NAME
#endif

#ifdef CAPTURE
#define CAPTURE_NAME Capture
#else
#define CAPTURE_NAME
#endif

#ifdef RECEIVE
#define RECEIVE_NAME Receive
#else
#define RECEIVE_NAME
#endif

#ifdef RENDERER
#define RENDERER_NAME Renderer
#else
#define RENDERER_NAME
#endif

#ifdef TRANSMITTERS
#define TRANSMITTERS_NAME Transmitters
#else
#define TRANSMITTERS_NAME
#endif

#ifdef PARAMETERS
#define PARAMETERS_NAME Parameters
#define PARAMETER_CONST
#else
#define PARAMETERS_NAME
#define PARAMETER_CONST const
#endif

#ifdef CAPTURE
static PARAMETER_CONST unsigned long captureEndingTimeout = DEFAULT_CAPTURE_ENDINGTIMEOUT; // milliseconds
//static const unsigned sensorNo = 1; // presently, only one supported
#endif
#ifdef RECEIVE
static PARAMETER_CONST unsigned long receiveEndingTimeout = DEFAULT_RECEIVE_ENDINGTIMEOUT; // milliseconds
#endif
#if defined(RECEIVE) | defined(CAPTURE)
static PARAMETER_CONST unsigned long beginTimeout = DEFAULT_BEGINTIMEOUT; // milliseconds
static PARAMETER_CONST uint16_t captureSize = DEFAULT_CAPTURESIZE;
#endif

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

#define modulesSupported EXPAND_AND_QUOTE(Base TRANSMIT_NAME CAPTURE_NAME RENDERER_NAME RECEIVE_NAME PARAMETERS_NAME PRONTO_NAME )
#ifndef PROGNAME
#define PROGNAME "MicroGirs"
#endif

#define okString "OK"
#define errorString "ERROR"
#define timeoutString "."

static void printVariable(Stream& stream, const char *variableName, unsigned long value) {
    stream.print(variableName);
    stream.print("=");
    stream.println(value);
}

#ifdef TRANSMIT

static bool sendIrSignal(const IrSignal &irSignal, unsigned int noSends=1) {
    if (noSends == 0)
        return false;
    IrSender *irSender =
#ifdef NON_MOD
            (irSignal.getFrequency() == 0) ? (IrSender*) new IrSenderNonMod(NON_MOD_PIN) :
#endif
            (IrSender*) IrSenderPwm::getInstance(true);

    irSender->sendIrSignal(irSignal, noSends);

#ifdef NON_MOD
    if (irSignal.getFrequency() == 0)
        delete irSender;
    else
#endif
        IrSenderPwm::deleteInstance();

    return true;
}

#endif // TRANSMIT

#if defined(RECEIVE) | defined(CAPTURE)
static void flushIn(Stream &stream) {
    while (stream.available())
        stream.read();
}
#endif

#ifdef RECEIVE

static void decodeOrDump(IrReader *irReader, Stream& stream) {
    if (irReader->isEmpty())
        stream.println(F(timeoutString));
    else
        irReader->dump(stream);
}

static bool receive(Stream& stream) {
    IrReceiverSampler *irReceiver = IrReceiverSampler::getInstance();
    if (irReceiver == NULL)
        irReceiver = IrReceiverSampler::newIrReceiverSampler(captureSize,
            IRRECEIVER_PIN, IRRECEIVER_PULLUP);
    if (irReceiver == NULL)
        return false;
    irReceiver->setEndingTimeout(receiveEndingTimeout);
    irReceiver->setBeginningTimeout(beginTimeout);
    irReceiver->setMarkExcess(IRRECEIVER_MARK_EXCESS);
    flushIn(stream);
    irReceiver->enable();

    while (!irReceiver->isReady() && stream.available() == 0)
        ;
    bool ready = irReceiver->isReady();
    irReceiver->disable();
    if (ready)
        decodeOrDump(irReceiver, stream);
    IrReceiverSampler::deleteInstance();
    return true;
}
#endif // RECEIVE

#ifdef CAPTURE

static bool capture(Stream& stream) {
    IrWidget *irWidget = IrWidgetAggregating::newIrWidgetAggregating(captureSize,
            IRCAPTURER_PULLUP);
    if (irWidget == NULL)
        stream.println(F("This cannot happen"));
    irWidget->setEndingTimeout(captureEndingTimeout);
    irWidget->setBeginningTimeout(beginTimeout);
    irWidget->setMarkExcess(IRSENSOR_MARK_EXCESS);
    irWidget->reset();
    flushIn(stream);
    irWidget->capture();

    if (!irWidget->isEmpty()) {
        // Trying to decode the capture does not make sense,
        // that is what "receive" is for.
        irWidget->dump(stream);
    } else
        stream.println(F(timeoutString));
    IrWidgetAggregating::deleteInstance();
    return true;
}
#endif // CAPTURE

void setup() {
#if defined(TRANSMIT)
    // Make sure that sender is quiet (if reset or such)
    IrSenderPwm::getInstance(true)->mute();
#endif

    Serial.begin(BAUD);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    Serial.println(F(PROGNAME " " VERSION));
    Serial.setTimeout(SERIAL_TIMEOUT);
}

static inline bool isPrefix(const String& cmd, const char *string) {
    return strncmp(cmd.c_str(), string, cmd.length()) == 0;
}

static inline bool isPrefix(const char *string, const String& cmd) {
    return strncmp(cmd.c_str(), string, strlen(string)) == 0;
}

bool isPrefix(const String& cmd, const __FlashStringHelper *pstring) {
    return strncmp_PF(cmd.c_str(), (uint_farptr_t) pstring, cmd.length()) == 0;
}

bool isPrefix(const __FlashStringHelper *pstring, const String& cmd) {
    return strncmp_PF(cmd.c_str(), (uint_farptr_t) pstring, strlen_PF((uint_farptr_t) pstring)) == 0;
}

static String readCommand(Stream& stream) {
    //flushIn(stream);
    while (stream.available() == 0)
        ;

    String line = stream.readStringUntil(EOLCHAR);
    line.trim();
    return line;
}

static bool processCommand(const String& line, Stream& stream) {
    Tokenizer tokenizer(line);
    String cmd = tokenizer.getToken();

    // Decode command
    if (cmd.length() == 0) {
        // empty command, do nothing
        stream.println(F(okString));
    } else

#ifdef CAPTURE
        if (cmd[0] == 'a' || cmd[0] == 'c') {
        capture(stream);
    } else
#endif // CAPTURE

        if (isPrefix(cmd, F("modules"))) {
        stream.println(F(modulesSupported));
    } else

#ifdef PARAMETERS
        if (cmd[0] == 'p') { // parameter
        String variableName = tokenizer.getToken();
        long value = tokenizer.getInt();
        unsigned long *variable32 = NULL;
        uint16_t *variable16 = NULL;
        uint8_t *variable8 = NULL;
#if defined(RECEIVE) || defined(CAPTURE)
           if (isPrefix(F("beg"), variableName))
            variable32 = &beginTimeout;
        else
#endif
#ifdef CAPTURE
            if (isPrefix(F("capturee"), variableName))
            variable32 = &captureEndingTimeout;
#endif
#ifdef RECEIVE
           if (isPrefix(F("receivee"), variableName))
            variable32 = &receiveEndingTimeout;
        else
#endif
#ifdef CAPTURE
        if (isPrefix(F("captures"), variableName)) {
        // TODO: check evenness of value
        variable16 = &captureSize;
        } else
#endif
        if (variable32 != NULL) {
            if (value != Tokenizer::invalid)
                *variable32 = value;

            printVariable(stream, variableName.c_str(), *variable32);
        } else if (variable16 != NULL) {
            if (value != Tokenizer::invalid)
                *variable16 = (uint16_t) value;

            printVariable(stream, variableName.c_str(), *variable16);
        } else if (variable8 != NULL) {
            if (value != Tokenizer::invalid)
                *variable8 = (uint8_t) value;

            printVariable(stream, variableName.c_str(), *variable8);
        } else
            stream.println(F("No such variable"));
    } else
#endif // PARAMETERS

#ifdef RECEIVE
        // TODO: option for force decoding off
        if (isPrefix(cmd, F("receive"))) { // receive
        bool status = receive(stream);
        if (!status)
            stream.println(F(errorString));
    } else
#endif // RECEIVE

#ifdef TRANSMIT
        if (cmd[0] == 's') { // send
        // TODO: handle unparsable data gracefully
        uint16_t noSends = (uint16_t) tokenizer.getInt();
        frequency_t frequency = tokenizer.getFrequency();
        uint16_t introLength = (uint16_t) tokenizer.getInt();
        uint16_t repeatLength = (uint16_t) tokenizer.getInt();
        uint16_t endingLength = (uint16_t) tokenizer.getInt();
        microseconds_t intro[introLength];
        microseconds_t repeat[repeatLength];
        microseconds_t ending[endingLength];
        for (uint16_t i = 0; i < introLength; i++)
            intro[i] = tokenizer.getMicroseconds();
        for (uint16_t i = 0; i < repeatLength; i++)
            repeat[i] = tokenizer.getMicroseconds();
        for (uint16_t i = 0; i < endingLength; i++)
            ending[i] = tokenizer.getMicroseconds();
        IrSignal irSignal(intro, introLength, repeat, repeatLength, ending, endingLength, frequency);
        bool status = sendIrSignal(irSignal, noSends); // waits
        stream.println(status ? F(okString) : F(errorString));
    } else
#endif // TRANSMIT

#ifdef PRONTO
        if (isPrefix(cmd, F("hex"))) { // pronto hex send
        uint16_t noSends = (uint16_t) tokenizer.getInt();
        String rest = tokenizer.getRest();
        IrSignal *irSignal = Pronto::parse(rest.c_str());
        bool status = false;
        if (irSignal != NULL) {
            status = sendIrSignal(*irSignal, noSends); // waits
            delete irSignal;
        }
        stream.println(status ? F(okString) : F(errorString));
    } else
#endif // PRONTO

#ifdef RENDERER
        if (cmd[0] == 't') { // transmit
        // TODO: handle unparseable data gracefully
        uint16_t noSends = (uint16_t) tokenizer.getInt();
        String protocol = tokenizer.getToken();
        const IrSignal *irSignal = NULL;
        if (isPrefix(protocol, F("nec1"))) {
            unsigned int D = (unsigned) tokenizer.getInt();
            unsigned int S = (unsigned) tokenizer.getInt();
            unsigned int F = (unsigned) tokenizer.getInt();
            irSignal = (F == Tokenizer::invalid)
                    ? Nec1Renderer::newIrSignal(D, S)
                    : Nec1Renderer::newIrSignal(D, S, F);
        } else if (isPrefix(protocol, F("rc5"))) {
            unsigned int D = (unsigned) tokenizer.getInt();
            unsigned int F = (unsigned) tokenizer.getInt();
            unsigned int T = (unsigned) tokenizer.getInt();
            irSignal = (T == Tokenizer::invalid)
                    ? Rc5Renderer::newIrSignal(D, F)
                    : Rc5Renderer::newIrSignal(D, F, T);
        } else {
            stream.print(F("no such protocol: "));
            stream.println(protocol);
        }
        bool status = false;
        if (irSignal != NULL) {
            status = sendIrSignal(*irSignal, noSends); // waits, blinks
            delete irSignal;
        }
        stream.println(status ? F(okString) : F(errorString));
    } else
#endif // RENDERER

        if (cmd[0] == 'v') { // version
        stream.println(F(PROGNAME " " VERSION));
    } else {
        stream.println(F(errorString));
    }

    return true;
}

void loop() {
    Stream& stream = Serial;
    String line = readCommand(stream);
    processCommand(line, stream);
}
