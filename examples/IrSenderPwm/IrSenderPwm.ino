// This sketch sends a raw signal using the PWM sender every 5 seconds.
// It requires an IR-Led connected to the sending pin
// (3 on Uno/Nano, 9 on Leonardo/Uno, 9 on Mega2560 etc...)

#include <IrSenderPwm.h>

static const frequency_t necFrequency = 38400U;
static const dutycycle_t dutyCycle = 30U;
static const unsigned long BAUD = 115200U;

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
    Serial.begin(BAUD);
    while (!Serial)
        ;
    //Serial.print("Output on pin ");
    //Serial.println(Board::getInstance()->getPwmPin(), DEC);
    if (Board::getInstance()->hasHardwarePwm())
        Serial.println(F("Hardware PWM available!"));
    else
        Serial.println(F("Hardware PWM NOT available, will be emulated in software."));
}

void loop() {
    Serial.print(F("Shooting @ pin "));
    Serial.print(IrSenderPwm::getInstance(true)->getPin(), DEC);
    Serial.print(F(" with duty cycle "));
    Serial.print(dutyCycle, DEC);
    Serial.println("%");
    IrSenderPwm::getInstance(true)->send(irSequence, necFrequency, dutyCycle);
    delay(5000);
}
