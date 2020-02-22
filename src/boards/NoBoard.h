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

#pragma once

#include "Board.h"

#define CURRENT_CLASS NoBoard
//#define HAS_HARDWARE_PWM 0

class NoBoard : public Board {
#define PWM_PIN Board::NO_PIN
public:
    NoBoard() {};

private:
    void TIMER_CONFIG_NORMAL() {};
    void TIMER_ENABLE_INTR() {};
    void TIMER_DISABLE_INTR() {};
    void TIMER_CONFIG_HZ(frequency_t hz __attribute__((unused)), dutycycle_t dutyCycle __attribute__((unused))) {};
    void TIMER_ENABLE_PWM() {};
    void TIMER_DISABLE_PWM() {};
};
