// This sketch combines sending and receiving.
// It is useful for testing by developers.
// It is NOT intended as a pedagogical example of usage of the library.

#include <Arduino.h>
#include <IrReceiverSampler.h>
#include <IrSenderPwm.h>

#ifdef ESP32
static constexpr pin_t RECEIVE_PIN = 4U;
#elif defined(ESP8266)
static constexpr pin_t RECEIVE_PIN = 2U;
#elif defined(ARDUINO_AVR_MICRO)
static constexpr pin_t RECEIVE_PIN = 10U;
#elif defined (ARDUINO_TEENSY30) || defined (ARDUINO_TEENSY32) // Teenex 3.0-3.2
static constexpr pin_t RECEIVE_PIN = 7U;
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

static constexpr frequency_t necFrequency = 38400U;

// NEC(1) 122 29 with no repetition; powers on many Yamaha receivers
static const microseconds_t array[]
#if HAS_FLASH_READ
        PROGMEM
#endif
        = {
    9024, 4512, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 1692,
    564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 1692,
    564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564,
    564, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 564, 564, 564,
    564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 1692, 564,
    1692, 564, 1692, 564, 39756
};

static const IrSequence* irSequence =
#if HAS_FLASH_READ
        IrSequence::readFlash(array, sizeof (array) / sizeof (microseconds_t));
#else
        new IrSequence(array, sizeof (array) / sizeof (microseconds_t));
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
    while(!Serial)
        ;
#if HAS_FLASH_READ
    Serial.println(F("Sending a signal from PROGMEM!"));
#else
    Serial.println(F("Sending a signal!"));
#endif
    IrSender *sender = IrSenderPwm::getInstance(true);
    sender->send(*irSequence, necFrequency);

    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
    Serial.println(F("Listening!"));
    receiver->enable();
}

void loop() {
    receiver->receive(); // combines enable, loop, disable
    if (receiver->isEmpty()) {
        Serial.println("timeout");
    } else {
        receiver->dump(Serial);
        Serial.println();
    }
}
