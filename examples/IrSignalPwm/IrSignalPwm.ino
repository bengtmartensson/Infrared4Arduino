// Send RC5 0/0 periodically

#include <Arduino.h>
#include <IrSenderPwm.h>

const microseconds_t array[] = {
    889, 889, 1778, 889, 889, 889, 889, 889, 889, 889, 889, 889, 889, 889, 889,
    889, 889, 889, 889, 889, 889, 889, 889, 889, 889, 65535 /* really 90886*/
};
IrSequence irSequence(array, sizeof (array) / sizeof (microseconds_t));

void setup() {
}

void loop() {
    IrSenderPwm::getInstance(true)->send(irSequence, 36000U);
    delay(2000);
}
