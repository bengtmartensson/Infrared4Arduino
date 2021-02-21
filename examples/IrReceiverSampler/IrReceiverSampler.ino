// This sketch demonstrates the IrReceiverSampler.
// It requires a demodulating sensor connected to pin RECEIVE_PIN.

#include <Arduino.h>
#include <IrReceiverSampler.h>

#ifdef ESP32
static constexpr pin_t RECEIVE_PIN = 4U;
#elif defined(ARDUINO_AVR_MICRO)
static constexpr pin_t RECEIVE_PIN = 10U;
#else
static constexpr pin_t RECEIVE_PIN = 5U;
#endif

static constexpr size_t BUFFERSIZE = 200U;
static constexpr uint32_t BAUD = 115200UL;

#ifdef ARDUINO_AVR_NANO
static constexpr pin_t IRRECEIVER_1_GND = 6U;
static constexpr pin_t IRRECEIVER_1_VCC = 7U;
#endif
#ifdef ARDUINO_AVR_MICRO
static constexpr pin_t IRRECEIVER_1_GND = 16U;
static constexpr pin_t IRRECEIVER_1_VCC = 14U;
#endif

static IrReceiver *receiver;

void setup() {
#ifdef IRRECEIVER_1_GND
    pinMode(IRRECEIVER_1_GND, OUTPUT);
    digitalWrite(IRRECEIVER_1_GND, LOW);
#endif

#ifdef IRRECEIVER_1_VCC
    pinMode(IRRECEIVER_1_VCC, OUTPUT);
    digitalWrite(IRRECEIVER_1_VCC, HIGH);
#endif
    Serial.begin(BAUD);
    while (!Serial)
        ;

    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
    Serial.print("Listening on pin ");
    Serial.println(receiver->getPin(), DEC);
}

void loop() {
    receiver->receive(); // combines enable, loop, disable
    if (receiver->isEmpty())
        Serial.println("timeout");
    else {
        receiver->dump(Serial);
        Serial.println();
    }
}
