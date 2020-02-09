#ifdef ESP8266

#include "Esp8266.h"
#include "IrReceiverSampler.h"

// "Idiot check"
#ifdef USE_DEFAULT_ENABLE_IR_IN
#error Must undef USE_DEFAULT_ENABLE_IR_IN
#endif


//extern "C" {
//#include "user_interface.h"
//#include "osapi.h"
//#define os_timer_arm_us(a, b, c) ets_timer_arm_new(a, b, c, 0)
////#include "gpio.h"
//}

os_timer_t Esp8266::timer;

void IrReceiverSampler::enable() {
    // Let the timers speak micros seconds
    system_timer_reinit();

    os_intr_lock();
    os_timer_setfn(&Esp8266::timer, IRTimer, NULL);

    os_timer_arm_us(&Esp8266::timer, IrReceiverSampler::microsPerTick, true);

    // Initialize state machine variables
    reset();

    os_intr_unlock();

    // Set pin modes
    pinMode(getPin(), INPUT);
}

#endif // ESP8266
