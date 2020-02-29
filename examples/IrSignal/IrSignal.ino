// Just a trivial exercise on creating and printing of IrSignal

#include <IrSignal.h>

static const uint32_t BAUD = 115200UL;

static const microseconds_t intro[] = {
    9041, 4507, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 573, 573,
    573, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 573, 573, 573,
    573, 573, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 573, 573,
    573, 573, 1694, 573, 1694, 573, 1694, 573, 573, 573, 573, 573, 1694,
    573, 1694, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 1694, 573,
    1694, 573, 44293U
};

static const microseconds_t repeat[] = { 9041, 2267, 573, 65535U };

IrSignal irSignal(intro, sizeof(intro)/sizeof(microseconds_t),
        repeat, sizeof(repeat)/sizeof(microseconds_t), 38400U);

void setup() {
    Serial.begin(BAUD);
    while(!Serial)
        ;
    irSignal.dump(Serial, true);
}

void loop() {
}
