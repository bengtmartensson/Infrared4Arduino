// This sketch sends a raw signal using the PWM sender every 5 seconds.
// It requires an IR-Led connected to the sending pin
// (3 on Uno/Nano, 9 on Leonardo/Uno, 9 on Mega2560 etc...)
// Just for the fun of it, the duty cycle is selected randomly.

#include <IrSenderPwm.h>

static constexpr frequency_t necFrequency = 38400U;
static constexpr unsigned long BAUD = 115200UL;
static constexpr pin_t PIN = 3U;

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
    if (Board::getInstance()->hasHardwarePwm())
        Serial.println(F("Hardware PWM available!"));
    else
        Serial.println(F("Hardware PWM NOT available, will be emulated in software."));
    randomSeed(analogRead(A0));
    irSender = IrSenderPwm::getInstance(true, PIN);
}

void loop() {
    dutycycle_t dutyCycle;dutyCycle = static_cast<dutycycle_t>(random(20,80));
    Serial.print(F("Shooting @ pin "));
    Serial.print(irSender->getPin(), DEC);
    Serial.print(F(" with duty cycle "));
    Serial.print(dutyCycle, DEC);
    Serial.println("%");
    irSender->send(irSequence, necFrequency, dutyCycle);
    delay(5000);
}
