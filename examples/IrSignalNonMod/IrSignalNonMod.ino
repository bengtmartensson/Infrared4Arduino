// Turn on and off an Arctech device with house code M = 13, device 8.

#include <Arduino.h>
#include <IrSenderNonMod.h>

const microseconds_t offData[] = {
    388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164, 388, 388,
    1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164,
    1164, 388, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164,
    388, 388, 1164, 1164, 388, 388, 1164, 388, 1164, 388, 11364
};
IrSequence off(offData, sizeof(offData)/sizeof(microseconds_t));

const microseconds_t onData[] = {
    388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164, 388, 388,
    1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164,
    1164, 388, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164,
    388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 11364
};
IrSequence on(onData, sizeof(onData)/sizeof(microseconds_t));

IrSenderNonMod sender(9U);

void setup() {
}

void loop() {
    sender.send(on);sender.send(on);
    delay(2000);
    sender.send(off);sender.send(off);
    delay(2000);
}
