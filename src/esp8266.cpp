#ifdef ESP8266

// NOTE (BM):
// This file is a stripped down version of IRremoteESP8266.cpp, stripped of
// everything that is not specific for the ESP8266, and renamed to esp8266.cpp.
// It (now) contains esp8266-specific functions that fit to IRremote.

// Some of the comments below are not relevant any longer,
// but kept for legal reasons.

/***************************************************
 * IRremote for ESP8266
 *
 * Based on the IRremote library for Arduino by Ken Shirriff
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see
 *   http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Modified by Paul Stoffregen <paul@pjrc.com> to support other boards and
 *   timers
 * Modified by Mitra Ardron <mitra@mitra.biz>
 * Added Sanyo and Mitsubishi controllers
 * Modified Sony to spot the repeat codes that some Sony's send
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by
 *   http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and
 *   other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 * Whynter A/C ARC-110WD added by Francesco Meschia
 * Global Cache IR format sender added by Hisham Khalifa
 *   (http://www.hishamkhalifa.com)
 * Coolix A/C / heatpump added by bakrus
 * Denon: sendDenon, decodeDenon added by Massimiliano Pinto
 *   (from https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp)
 * Kelvinator A/C and Sherwood added by crankyoldgit
 * Mitsubishi A/C added by crankyoldgit
 *     (derived from https://github.com/r45635/HVAC-IR-Control)
 *
 * Updated by markszabo (https://github.com/markszabo/IRremoteESP8266) for
 *   sending IR code on ESP8266
 * Updated by Sebastien Warin (http://sebastien.warin.fr) for receiving IR code
 *   on ESP8266
 *
 *  GPL license, all text above must be included in any redistribution
 ****************************************************/

#include "IrReceiverSampler.h"
#include "boarddefs.h"

extern "C" {
	#include "user_interface.h"
	#include "gpio.h"
}

#ifdef USE_DEFAULT_ENABLE_IR_IN
#error Must undef USE_DEFAULT_ENABLE_IR_IN
#endif

//// ISR State-Machine : Receiver States
//#define STATE_IDLE      2
//#define STATE_MARK      3
//#define STATE_SPACE     4
//#define STATE_STOP      5
//#define STATE_OVERFLOW  6

//const unsigned int RAWBUF = 501;
//
//typedef struct {
//    // The fields are ordered to reduce memory over caused by struct-padding
//    uint8_t rcvstate; // State Machine state
//    uint8_t recvpin; // Pin connected to IR data from detector
//    uint8_t blinkpin;
//    uint8_t blinkflag; // true -> enable blinking of pin on IR processing
//    /*uint8_t*/ uint16_t rawlen; // counter of entries in rawbuf
//    unsigned int timer; // State timer, counts 50uS ticks.
//    /*unsigned int*/ microseconds_t rawbuf[RAWBUF]; // raw data
//    uint8_t overflow; // Raw buffer overflow occurred
//} irparams_t;
//
//volatile irparams_t irparams;

// IRtimer ---------------------------------------------------------------------
// This class performs a simple time in useconds since instantiated.
// Handles when the system timer wraps around (once).

//#define ICACHE_FLASH_ATTR

class IRtimer {
public:
	IRtimer();
	void reset();
	uint32_t elapsed();
private:
	uint32_t start;
};

IRtimer::IRtimer()
{
	reset();
}

void ICACHE_FLASH_ATTR IRtimer::reset()
{
	start = micros();
}

uint32_t ICACHE_FLASH_ATTR IRtimer::elapsed()
{
	uint32_t now = micros();
	if (start <= now) // Check if the system timer has wrapped.
		return(now - start); // No wrap.
	else
		return(0xFFFFFFFF - start + now); // Has wrapped.
}

static ETSTimer timer;

static void ICACHE_RAM_ATTR read_timeout(void *arg __attribute__((unused)))
{
	os_intr_lock();
        IrReceiverSampler *recv = IrReceiverSampler::getInstance();
//	if (irparams.rawlen) {
//		irparams.rcvstate = STATE_STOP;
//	}
        if (recv->dataLength) {
		recv->receiverState = IrReceiverSampler::STATE_STOP;
	}
	os_intr_unlock();
}

static void ICACHE_RAM_ATTR gpio_intr() {
    uint32_t now = system_get_time();
    uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
    static uint32_t start = 0;

    os_timer_disarm(&timer);
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
    
    IrReceiverSampler *recv = IrReceiverSampler::getInstance();
   
    //	if (irparams.rawlen >= RAWBUF) {
    //		irparams.overflow = true;
    //		irparams.rcvstate = STATE_STOP;
    //	}
 
    if (recv->dataLength >= recv->getBufferSize())
        // Buffer full
        recv->receiverState = IrReceiverSampler::STATE_STOP;

    //	if (irparams.rcvstate == STATE_STOP) {
    //		return;
    //	}

    if (recv->receiverState == IrReceiverSampler::STATE_STOP)
        return;

    //	if (irparams.rcvstate == STATE_IDLE) {
    //		irparams.overflow = false;
    //		irparams.rcvstate = STATE_MARK;
    //		irparams.rawbuf[irparams.rawlen++] = 1;
    //	} else {
    //		if (now < start)
    //			irparams.rawbuf[irparams.rawlen++] = (0xFFFFFFFF - start + now) / USECPERTICK + 1;
    //		else
    //			irparams.rawbuf[irparams.rawlen++] = (now - start) / USECPERTICK + 1;
    //	}
    if (recv->receiverState == IrReceiverSampler::STATE_IDLE) {
        //irparams.overflow = false;
        recv->receiverState = IrReceiverSampler::STATE_MARK;
        recv->dataLength = 0;
        recv->timer = 0;
    } else
        recv->durationData[recv->dataLength++] = ((now < start) ? (0xFFFFFFFF - start + now) : (now - start)) / USECPERTICK + 1;
    

    start = now;
#define ONCE 0
    os_timer_arm(&timer, 15, ONCE);
}

//void ICACHE_FLASH_ATTR IRrecv::enableIRIn()

void ICACHE_FLASH_ATTR IrReceiverSampler::enable() {
    // initialize state machine variables
    //	irparams.rcvstate = STATE_IDLE;
    //	irparams.rawlen = 0;
    reset();

    // Initialize timer
    os_timer_disarm((ETSTimer*)& timer);
    os_timer_setfn((ETSTimer*)& timer, (os_timer_func_t *) read_timeout, NULL);

    // Attach Interrupt
    attachInterrupt(IrReceiverSampler::getInstance()->getPin(), gpio_intr, CHANGE);
}

// This could probably be useful, presently not enabled.
//void ICACHE_FLASH_ATTR IRrecv::disableIRIn()
//{
//	os_timer_disarm(&timer);
//	detachInterrupt(irparams.recvpin);
//}

#endif // ESP8266
