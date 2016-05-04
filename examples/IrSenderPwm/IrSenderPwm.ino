// This sketch sends a raw signal using the PWM sender every 5 seconds.
// It requires an IR-Led connected to the sending pin
// (3 on Uno/Nano, 9 on Leonardo/Uno, 9 on Mega2560 etc...)

#include <IrSenderPwm.h>

static const frequency_t necFrequency = 38400U;

// NEC(1) 122 29 with no repetition; powers on many Yamaha receivers
static const microseconds_t array[] = {
   9024, 4512, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 1692,
   564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 1692,
   564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564,
   564, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 564, 564, 564,
   564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 1692, 564,
   1692, 564, 1692, 564, 39756
};

static const IrSequence irSequence(array, sizeof(array) / sizeof(microseconds_t));

void setup() {
}

void loop() {
    IrSenderPwm::getInstance(true)->send(irSequence, necFrequency);
    delay(5000);
}
