// Send RC5 0/0 periodically

#include <Arduino.h>
#include <IrSenderPwm.h>

const microseconds_t sequence[] = {
    889, 889, 1778, 889, 889, 889, 889, 889, 889, 889, 889, 889, 889, 889, 889,
    889, 889, 889, 889, 889, 889, 889, 889, 889, 889, 65535 /* really 90886*/
};

void setup() {
}

void loop() {
    IrSenderPwm::getInstance(true)->send(sequence, sizeof (sequence) / sizeof (microseconds_t), 36000U);
    delay(2000);
}
