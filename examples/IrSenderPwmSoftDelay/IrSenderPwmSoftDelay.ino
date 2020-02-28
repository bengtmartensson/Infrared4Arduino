// This sketch sends a raw signal using the PWM sender every 5 seconds.
// It requires an IR-Led connected to the sending pin
// (3 on Uno/Nano, 9 on Leonardo/Uno, 9 on Mega2560 etc...)

#include <IrSenderPwmSoftDelay.h>

static const frequency_t necFrequency = 38400U;
static const pin_t pin = 3U;
static const uint32_t BAUD = 115200UL;

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
IrSender* irSender;

void setup() {
    Serial.begin(BAUD);
    while (!Serial)
        ;
    irSender = new IrSenderPwmSoftDelay(pin);
}

void loop() {
    Serial.print(F("Sending @ pin "));
    Serial.println(pin, DEC);
    irSender->send(irSequence, necFrequency);
    delay(5000);
}
