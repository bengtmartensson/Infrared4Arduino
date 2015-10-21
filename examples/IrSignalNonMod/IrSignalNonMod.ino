// Turn on and off an Arctech device with house code M = 13, device 8.

#include <Arduino.h>
#include <IrSenderNonMod.h>

const microseconds_t off[] = {
    388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164, 388, 388,
    1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164,
    1164, 388, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164,
    388, 388, 1164, 1164, 388, 388, 1164, 388, 1164, 388, 11364
};

const microseconds_t on[] = {
    388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164, 388, 388,
    1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164,
    1164, 388, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164,
    388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 11364
};

IrSenderNonMod sender(3U);

void setup() {
}

void loop() {
    sender.send(on, sizeof(on)/sizeof(microseconds_t));
    sender.send(on, sizeof(on)/sizeof(microseconds_t));
    delay(2000);
    sender.send(off, sizeof(off)/sizeof(microseconds_t));
    sender.send(off, sizeof(on)/sizeof(microseconds_t));
    delay(2000);
}
