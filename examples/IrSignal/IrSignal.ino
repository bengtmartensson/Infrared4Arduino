// Just a trivial exercise on creating and printing of IrSignal

#include <IrSignal.h>
#include <Board.h> // for HAS_FLASH_READ

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

#if HAS_FLASH_READ
static const microseconds_t intro_pm[] PROGMEM = {
    9041, 4507, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 573, 573,
    573, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 573, 573, 573,
    573, 573, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 573, 573,
    573, 573, 1694, 573, 1694, 573, 1694, 573, 573, 573, 573, 573, 1694,
    573, 1694, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 1694, 573,
    1694, 573, 44293U
};

static const microseconds_t repeat_pm[] PROGMEM = { 9041, 2267, 573, 65535U };

IrSignal* irSignal_pm = IrSignal::readFlash(intro_pm, sizeof(intro_pm)/sizeof(microseconds_t),
        repeat_pm, sizeof(repeat_pm)/sizeof(microseconds_t), 38400U);
#endif

void setup() {
    Serial.begin(BAUD);
    while(!Serial)
        ;
    irSignal.dump(Serial, true);
#ifdef HAS_FLASH_READ
    irSignal_pm->dump(Serial, true);
#endif
}

void loop() {
}
