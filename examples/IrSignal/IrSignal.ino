#include <Arduino.h>
#include <IrSignal.h>

microseconds_t intro[] = {
    +9041, -4507, 573, -573, 573, -573, 573, -1694, 573, -1694, 573, -573, 573,
    -573, 573, -573, 573, -573, 573, -573, 573, -1694, 573, -573, 573, -573,
    573, -573, 573, -1694, 573, -573, 573, -573, 573, -573, 573, -573, 573,
    -573, 573, -1694, 573, -1694, 573, -1694, 573, -573, 573, -573, 573, -1694,
    573, -1694, 573, -1694, 573, -573, 573, -573, 573, -573, 573, -1694, 573,
    -1694, 573, -44293
};

microseconds_t repeat[] = { +9041, -2267, 573, -96193};

IrSignal irSignal(intro, sizeof(intro)/sizeof(microseconds_t),
        repeat, sizeof(repeat)/sizeof(microseconds_t),
        NULL, 0, 38400U);

void setup() {
    Serial.begin(9600);
    Serial.println("Testing...");
    irSignal.dump(Serial);
}

void loop() {

}
