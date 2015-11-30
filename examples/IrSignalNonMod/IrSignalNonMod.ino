// Turn on and off an Arctech device with house code M = 13, device 8.

#include <IrSenderNonMod.h>

static const microseconds_t offData[] = {
    388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164, 388, 388,
    1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164,
    1164, 388, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164,
    388, 388, 1164, 1164, 388, 388, 1164, 388, 1164, 388, 11364
};
static const IrSequence off(offData, sizeof(offData)/sizeof(microseconds_t));

static const microseconds_t onData[] = {
    388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164, 388, 388,
    1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 1164,
    1164, 388, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 388, 1164, 1164,
    388, 388, 1164, 1164, 388, 388, 1164, 1164, 388, 388, 11364
};
static const IrSequence on(onData, sizeof(onData)/sizeof(microseconds_t));

static IrSenderNonMod sender(9U);

void setup() {
}

void loop() {
    sender.send(on, 0);
    delay(2000);
    sender.send(off, 0);
    delay(2000);
}
