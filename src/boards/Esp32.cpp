#ifdef ESP32

#include "IrReceiverSampler.h"
#if 1
//hw_timer_t *Xtimer;
//void IRTimer(); // defined in IrReceiverSampler.cpp, masqueraded as ISR(TIMER_INTR_NAME)
#endif
//+=============================================================================
// initialization
//
//
//void IrReceiverSampler::enable() {
//    // Interrupt Service Routine - Fires every 50uS
//    // ESP32 has a proper API to setup timers, no weird chip macros needed
//    // simply call the readable API versions :)
//    // 3 timers, choose #1, 80 divider nanosecond precision, 1 to count up
//    Xtimer = timerBegin(1, 80, 1);
//    timerAttachInterrupt(Xtimer, &IRTimer, true);
//    // every 50ns, autoreload = true
//    timerAlarmWrite(Xtimer, 50, true);
//    timerAlarmEnable(Xtimer);
//
//    // Initialize state machine variables
//    reset();
//
//    // Set pin modes
//    pinMode(getPin(), INPUT);
//}

#endif // ESP32
