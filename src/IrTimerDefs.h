#ifndef IRTIMERDEFS_H
#define IRTIMERDEFS_H

/**
 * @file IrTimerDefs.h
 * @brief Definition of timers etc is encapsulated in this file.
 */

#include "boarddefs.h"

#ifndef ARDUINO

// Just dummy junk to allow builds  outside of the Arduino environment.

#undef TIMER_CONFIG_NORMAL
#define TIMER_CONFIG_NORMAL()

#undef TIMER_ENABLE_INTR
#define TIMER_ENABLE_INTR

#undef TIMER_DISABLE_INTR
#define TIMER_DISABLE_INTR

#define ISR(x) void functionThatIDoNotNeed()

//#define IR_SEND_PWM_PIN 0

#undef TIMER_DISABLE_PWM
#define TIMER_DISABLE_PWM

#undef TIMER_ENABLE_PWM
#define TIMER_ENABLE_PWM

#undef TIMER_CONFIG_KHZ
#define TIMER_CONFIG_KHZ(x) /* Empty */

#endif // ! ARDUINO

#endif /* IRTIMERDEFS_H */

