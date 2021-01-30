/*
Copyright (C) 2020 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file Esp32.h
 *
 * @brief Hardware dependent definitions for Esp32 boards.
 * Based upon https://github.com/z3t0/Arduino-IRremote/pull/540/files
 * by Sensorslot (Andreas Spiess), as well as
 * https://github.com/anothermist/LIBRARIES/blob/master/IRremote/esp32.cpp
 */

#pragma once

#define CURRENT_CLASS Esp32

#define HAS_FLASH_READ      0
#define HAS_HARDWARE_PWM    1
#define HAS_SAMPLING        1
#define HAS_INPUT_CAPTURE   0

#define STRCPY_PF_CAST(x) (x)

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define LEDCHANNEL 0
#define TIMER_SIZE 8

#ifdef ISR
#undef ISR
#endif
#define ISR(f) void ICACHE_RAM_ATTR IRTimer()

void IRTimer(); // defined in IrReceiverSampler.cpp, masqueraded as ISR(TIMER_INTR_NAME)

#define PWM_PIN 5

class Esp32 : public Board {
public:

    Esp32() {
    };

private:

    static hw_timer_t* timer;
    uint8_t onValue;

    void timerEnableIntr() {
        // Interrupt Service Routine - Fires every 50uS
        // ESP32 has a proper API to setup timers, no weird chip macros needed
        // simply call the readable API versions :)
        // 3 timers, choose #1, 80 divider nanosecond precision, 1 to count up
        Esp32::timer = timerBegin(1, 80, 1);
        timerAttachInterrupt(Esp32::timer, IRTimer, true);
        // every 50 microseconds, autoreload = true
        timerAlarmWrite(Esp32::timer, microsPerTick, true);
        timerAlarmEnable(Esp32::timer);
    };

    void timerDisableIntr() {
        if (Esp32::timer != nullptr) {
            timerEnd(Esp32::timer);
            timerDetachInterrupt(Esp32::timer);
        }
    };

    void timerEnablePwm() {
        ledcWrite(LEDCHANNEL, onValue);
    };

    void timerDisablePwm() {
        ledcWrite(LEDCHANNEL, 0);
    };

    void timerConfigHz(frequency_t frequency, dutycycle_t dutyCycle) {
        onValue = static_cast<uint8_t>(256U * dutyCycle / 100U);
        ledcSetup(LEDCHANNEL, frequency, TIMER_SIZE);
        ledcAttachPin(PWM_PIN, LEDCHANNEL);
    };

    void timerConfigNormal() {
    }
};