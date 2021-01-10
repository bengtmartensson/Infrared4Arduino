#pragma once

#ifdef ARDUINO
#include_next <Arduino.h>

#else // ! ARDUINO

#include <stdint.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

#include "InfraredTypes.h"
#include "PinModeStatus.h"

typedef void *uint_farptr_t;
typedef void __FlashStringHelper;

#define REPORT_TIMES
//#define REAL_TIME

#define String std::string

#define indexOf find
#define charAt at
#define substring substr

#define F(x) x

#define A0 100
#define A1 101
#define A2 102
#define A3 103
#define A4 104
#define A5 101
#define A6 106
#define A7 107
#define  LED_BUILTIN 13

// Can't use pin_t yet
extern uint8_t currentWritePin; // SIL.cpp
extern struct timeval simulatedTime;

static timeval getTimeOfDay() {
#ifdef REAL_TIME
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
#else
    return simulatedTime;
#endif
}

static struct timeval last = getTimeOfDay();
static PinStatus lastValue = LOW;

inline void pinMode(uint8_t pin, PinMode mode) {
    std::cout << "pinMode(" << (int) pin << ", "
            << (mode == INPUT ? "INPUT" : mode == OUTPUT ? "OUTPUT" : mode == INPUT_PULLUP ? "INPUT_PULLUP" : "INPUT_PULLDOWN")
            << ")" << std::endl;
    if (mode == OUTPUT) {
        currentWritePin = pin;
        lastValue = LOW;
    }
}

inline void delayMicroseconds(unsigned int t) {
#ifdef REAL_TIME
    usleep(t);
#else
    simulatedTime.tv_sec += t/1000000U;
    simulatedTime.tv_usec += t % 1000000U;
    while (simulatedTime.tv_usec > 1000000U) {
        simulatedTime.tv_usec -= 1000000U;
        simulatedTime.tv_sec++;
    }
#endif
}

inline void delay(unsigned long t) {
    delayMicroseconds(1000U * t);
}

inline void noInterrupts() {}
inline void interrupts() {}
inline void yield() {}

inline unsigned long micros() {
    struct timeval tv = getTimeOfDay();
    // Probably overflows, but this should be OK in 99.99% of all cases, which is enough here.
    return 1000000UL * tv.tv_sec + tv.tv_usec;
}

inline unsigned long millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // Probably overflows, but this should be OK in 99.99% of all cases, which is enough here.
    return 1000*tv.tv_sec + tv.tv_usec/1000;
}

inline uint8_t digitalRead(uint8_t pin __attribute__((unused))) {
    return 0;
}

inline void digitalWrite(uint8_t pin __attribute__((unused)), PinStatus value) {
#ifdef REPORT_TIMES
//    if (pin != currentPin)
//        return;
    struct timeval now = getTimeOfDay();
    if (lastValue == -1)
        if (value == LOW)
            return;
        else {
            lastValue = value;
            last = now;
        }
    else if (lastValue == (value != LOW))
        return;
    else {
        char sign = value ? '-' : '+';
        std::cout << sign << 1000000UL * (now.tv_sec - last.tv_sec) + (now.tv_usec - last.tv_usec) << " ";
        last = now;
        lastValue = value;
    }
#else
    std::cout << "digitalWrite(" << (int) pin << ", "
            << (value == LOW ? "LOW" : "HIGH") << ")" << std::endl;
#endif
}


#define F_CPU 16000000 // Good default, correct for Unu etc

// Stream

class Stream {
private:
    std::ostream& stream;
public:
    Stream(std::ostream& stream_) : stream(stream_) {};
    void print(char c) { stream << c; };
    void print(const char *c) { stream << c; };
    void print(const std::string& string) { stream << string; };
    void print(int x) { stream << x; };
    void print(uint32_t  x) { stream << x; };
    void print(int x, std::ios_base& manipulator(std::ios_base&)) { stream << manipulator << x; };
    void println() { stream << std::endl; };
    void println(const std::string& string) { stream << string << std::endl; };
    void println(long x) { stream << x << std::endl; }
    int available() { return 0; }
};

#define DEC std::dec

#endif // ! ARDUINO
