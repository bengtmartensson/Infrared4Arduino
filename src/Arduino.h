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

#ifdef __GNUC__
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

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

extern uint8_t CS00;
extern uint8_t CS01;
extern uint8_t CS02;
extern uint8_t CS11;
extern uint8_t DDRB;
extern uint8_t ICES1;
extern uint8_t ICF1;
extern uint8_t ICNC1;
extern uint8_t ICR1;
extern uint8_t OCF1A;
extern uint8_t OCR1A;
extern uint8_t PORTB;
extern uint8_t PRR;
extern uint8_t PRTIM1;
extern uint8_t SREG;
extern uint8_t TCCR0B;
extern uint8_t TCCR1A;
extern uint8_t TCCR1B;
extern uint8_t TCNT1;
extern uint8_t TIFR1;
extern uint8_t TOV1;

inline uint8_t digitalRead(uint8_t pin UNUSED) { return 0; };
inline void digitalWrite(uint8_t pin, uint8_t value) {
    std::cout << "digitalWrite(" << (int) pin << ", "
            << (value == 0 ? "LOW" : "HIGH") << ")" << std::endl;
};

inline void pinMode(uint8_t pin, int mode) {
    std::cout << "pinMode(" << (int) pin << ", "
            << (mode == 0 ? "INPUT" : mode == 1 ? "OUTPUT" : "INPUT_PULLUP")
            << ")" << std::endl;
};

inline void delayMicroseconds(unsigned int t) { usleep(t); };

inline void delay(unsigned long t) {
    usleep(1000U * t);
};

inline void noInterrupts() {};
inline void interrupts() {};

inline unsigned long micros() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // Probably overflows, but this should be OK in 99.99% of all cases, which is enough here.
    return 1000000 * tv.tv_sec + tv.tv_usec;
}

inline unsigned long millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // Probably overflows, but this should be OK in 99.99% of all cases, which is enough here.
    return 1000*tv.tv_sec + tv.tv_usec/1000;
}

#define HIGH 1
#define LOW 0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define F_CPU 16000000

#define _BV(bit) (1 << (bit))
#define _SFR_MEM_ADDR(sfr) (/*(uint16_t)*/ &(sfr))
#define _SFR_ADDR(sfr) _SFR_MEM_ADDR(sfr)
#define _SFR_BYTE(sfr) _MMIO_BYTE(_SFR_ADDR(sfr))
#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))

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
