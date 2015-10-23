#include "Nec1Renderer.h"

Nec1Renderer::~Nec1Renderer() {
}

const microseconds_t Nec1Renderer::repeat[repeatLength] = { 9024, 2256, 564, 65535 };

void Nec1Renderer::init(unsigned int D, unsigned int S, unsigned int F) {
    unsigned int i = 0;
    uint32_t sum = 9024 + 4512 + 564;
    intro[i++] = 9024;
    intro[i++] = 4512;
    lsbByte(i, sum, D);
    lsbByte(i, sum, S);
    lsbByte(i, sum, F);
    lsbByte(i, sum, 255-F);
    intro[i++] = 564;
    intro[i++] = (microseconds_t) (108000 - sum);
}

const IrSignal& Nec1Renderer::render() const {
    return *(new IrSignal(frequency, introLength, repeatLength, endingLength, intro, repeat, NULL));
}

void Nec1Renderer::lsbByte(unsigned int& i, uint32_t& sum, unsigned int X) {
    for (unsigned int index = 0; index < 8; index++) {
        process(i, sum, X & 1);
        X >>= 1;
    }
}

void inline Nec1Renderer::process(unsigned int& i, uint32_t& sum, unsigned int data) {
    intro[i++] = 564;
    intro[i++] = data ? 1692 : 564;
    sum += data ? 564+1692 : 564+564;
}
