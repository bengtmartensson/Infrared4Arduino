// This sketch demonstrates the usage of the non-modulating sender in inverting
// mode. It can thus be used to emulate an inverting IR demodulator, like TSOP-*.

#include <IrSenderNonMod.h>

static constexpr pin_t NON_MOD_PIN = 9U;

static const microseconds_t data[] = {
    9041,4507,573,573,573,573,573,1694,573,1694,573,573,573,573,573,573,573,
    573,573,573,573,1694,573,573,573,573,573,573,573,1694,573,573,573,573,573,
    573,573,573,573,573,573,1694,573,1694,573,1694,573,573,573,573,573,1694,
    573,1694,573,1694,573,573,573,573,573,573,573,1694,573,1694,573,44293
};
static const IrSequence seq(data, sizeof(data)/sizeof(microseconds_t));

static IrSenderNonMod sender(NON_MOD_PIN, true);

void setup() {
}

void loop() {
    sender.sendNonModulated(seq);
    delay(2000);
}
