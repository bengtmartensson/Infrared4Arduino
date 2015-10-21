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

IrSignal irSignal(38400U, sizeof(intro)/sizeof(microseconds_t), sizeof(repeat)/sizeof(microseconds_t), 0,
        intro, repeat, NULL);

void setup() {
    Serial.begin(9600);
    Serial.println("Testing...");
    irSignal.dump(Serial);
}

void loop() {

}
