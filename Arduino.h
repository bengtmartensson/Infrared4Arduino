#ifndef ARDUINO_H
#define ARDUINO_H

#ifdef ARDUINO
#include_next <Arduino.h>

#else // ! ARDUINO

#include <stdint.h>
#include <stdio.h>
#include <string>
#include <iostream>
//#include <iomanip>

typedef bool boolean;

#define String std::string

#define F(x) x

inline uint8_t digitalRead(uint8_t pin) { return 0; };
inline void digitalWrite(uint8_t pin, uint8_t value) {};

inline void pinMode(uint8_t pin, int x) {};

inline long micros() { return 0L; }
inline void delayMicroseconds(unsigned int t) {};
inline void delay(unsigned long t) {};

inline void noInterrupts() {};
inline void interrupts() {};

#define HIGH 1
#define LOW 0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define F_CPU 16000000

// Stream

class Stream {
private:
    std::ostream& stream;
public:
    Stream(std::ostream& stream_) : stream(stream_) {};
    void print(char c) { stream << c; };
    void print(const char *c) { stream << c; };
    void print(int x) { stream << x; };
    void print(int x, std::ios_base& manipulator(std::ios_base&)) { stream << manipulator << x; };
    void println() { stream << std::endl; };
    void println(const std::string& string) { stream << string << std::endl; };
    void println(long x) { stream << x << std::endl; }
};

#define DEC std::dec

#endif // ! ARDUINO

#endif // ARDUINO_H
