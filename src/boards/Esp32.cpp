#ifdef ESP32

#include "IrReceiverSampler.h"

// "Idiot check"
#ifdef USE_DEFAULT_ENABLE_IR_IN
#error Must undef USE_DEFAULT_ENABLE_IR_IN
#endif

hw_timer_t* Esp32::timer = NULL;

void IrReceiverSampler::enable() {
    // Interrupt Service Routine - Fires every 50uS
    // ESP32 has a proper API to setup timers, no weird chip macros needed
    // simply call the readable API versions :)
    // 3 timers, choose #1, 80 divider nanosecond precision, 1 to count up
    Esp32::timer = timerBegin(1, 80, 1);
    timerAttachInterrupt(Esp32::timer, IRTimer, true);
    // every 50 microseconds, autoreload = true
    timerAlarmWrite(Esp32::timer, IrReceiverSampler::microsPerTick, true);
    timerAlarmEnable(Esp32::timer);

    // Initialize state machine variables
    reset();

    // Set pin modes
    pinMode(getPin(), INPUT);
}

#endif // ESP32
