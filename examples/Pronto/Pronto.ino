// Demonstrates the Pronto class: Three different forms of parsing
// (RAM string, explicit PROGMEM, F(.)-form),
// as well as the output function.

#include <Pronto.h>
#include <Nec1Renderer.h>

#define PRONTOSTRING "0000 006C 0022 0002 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 015B 0057 0016 0E6C"
uint32_t BAUD = 115200UL;

const IrSignal *irSignal;
const char PROGMEM data[] = PRONTOSTRING;

void setup() {
    Serial.begin(BAUD);
    while(!Serial)
        ;

    // Read a pronto string from RAM (normally highly wasteful with RAM).
    irSignal = Pronto::parse(PRONTOSTRING);
    irSignal->dump(Serial, true);

#if HAS_FLASH_READ
    // Read a signal from (explicit) PROGMEM
    irSignal = Pronto::parse_PF(data);
    irSignal->dump(Serial, true);

    // Use the F(.) form
    irSignal = Pronto::parse(F(PRONTOSTRING));
    irSignal->dump(Serial, true);
#endif

    irSignal = Nec1Renderer::newIrSignal(122, 27); // volume_down for Yahama receivers
    Serial.println(Pronto::toProntoHex(*irSignal));
    Pronto::dump(Serial, *irSignal);
    Serial.println();
}

void loop() {
}
