// This sketch sends a raw signal using the soft PWM sender every 5 seconds.
// It requires an IR-Led connected to the sending pin
// On slow processors, it may produce erroneous result,
// in particular modulation frequency.

#include <IrSenderPwmSpinWait.h>

static constexpr frequency_t necFrequency = 38400U;
static constexpr pin_t pin =
#ifdef ESP8266
        4U; // D2 on ESP8266
#else
        3U;
#endif
static constexpr uint32_t BAUD = 115200UL;

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
static IrSender* irSender;

void setup() {
    Serial.begin(BAUD);
    while (!Serial)
        ;
    irSender = new IrSenderPwmSpinWait(pin);
}

void loop() {
    Serial.print(F(" sending @ pin "));
    Serial.println(pin, DEC);
    irSender->send(irSequence, necFrequency);
    delay(5000);
}
