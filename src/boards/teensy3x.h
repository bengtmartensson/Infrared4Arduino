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
 * @file teensy3x.h
 *
 * @brief Hardware dependent definitions for Leonardo like boards like
 * Leonardo and Micro.
 */

#pragma once

#define USE_DEFAULT_ENABLE_IR_IN

#define TIMER_RESET ({     \
	uint8_t tmp __attribute__((unused)) = CMT_MSC; \
	CMT_CMD2 = 30;         \
})

#define TIMER_ENABLE_PWM  do {                                         \
	CORE_PIN5_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_DSE | PORT_PCR_SRE;  \
} while(0)

#define TIMER_DISABLE_PWM  do {                                        \
	CORE_PIN5_CONFIG = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;  \
} while(0)

#define TIMER_ENABLE_INTR   NVIC_ENABLE_IRQ(IRQ_CMT)
#define TIMER_DISABLE_INTR  NVIC_DISABLE_IRQ(IRQ_CMT)
#define TIMER_INTR_NAME     cmt_isr

//-----------------
#ifdef ISR
#undef ISR
#endif
#define  ISR(f)  void f(void)

//-----------------
#define CMT_PPS_DIV  ((F_BUS + 7999999) / 8000000)
#if F_BUS < 8000000
#error IRremote requires at least 8 MHz on Teensy 3.x
#endif

#define TIMER_CONFIG_KHZ(val) ({ 	 \
	SIM_SCGC4 |= SIM_SCGC4_CMT;      \
	SIM_SOPT2 |= SIM_SOPT2_PTD7PAD;  \
	CMT_PPS    = CMT_PPS_DIV - 1;    \
	CMT_CGH1   = ((F_BUS / CMT_PPS_DIV / 3000) + ((val)/2)) / (val); \
	CMT_CGL1   = ((F_BUS / CMT_PPS_DIV / 1500) + ((val)/2)) / (val); \
	CMT_CMD1   = 0;                  \
	CMT_CMD2   = 30;                 \
	CMT_CMD3   = 0;                  \
	CMT_CMD4   = 0;                  \
	CMT_OC     = 0x60;               \
	CMT_MSC    = 0x01;               \
})

#define TIMER_CONFIG_NORMAL() ({  \
	SIM_SCGC4 |= SIM_SCGC4_CMT;   \
	CMT_PPS    = CMT_PPS_DIV - 1; \
	CMT_CGH1   = 1;               \
	CMT_CGL1   = 1;               \
	CMT_CMD1   = 0;               \
	CMT_CMD2   = 30;              \
	CMT_CMD3   = 0;               \
	CMT_CMD4   = (F_BUS / 160000 + CMT_PPS_DIV / 2) / CMT_PPS_DIV - 31; \
	CMT_OC     = 0;               \
	CMT_MSC    = 0x03;            \
})

#define SEND_PIN  5
