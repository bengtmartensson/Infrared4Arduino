#include "Nec1Renderer.h"

// NOTE: writing intro[i++] = ... produces wrong result, compiler bug?
// (Adding a print statement immediately after, and it works :-~)
// So let's write intro[i] = ...; i++ at least for now.

Nec1Renderer::~Nec1Renderer() {
}

const microseconds_t Nec1Renderer::repeat[repeatLength] = { 9024, 2256, 564, 65535 };

void Nec1Renderer::init(unsigned int D, unsigned int S, unsigned int F) {
    unsigned int i = 0;
    uint32_t sum = 9024 + 4512 + 564;
    intro[i] = 9024; i++;
    intro[i] = 4512; i++;
    lsbByte(i, sum, D);
    lsbByte(i, sum, S);
    lsbByte(i, sum, F);
    lsbByte(i, sum, 255-F);
    intro[i] = 564; i++;
    intro[i] = (microseconds_t) (108000 - sum); i++;
    setup(intro, introLength, repeat, repeatLength, frequency);
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
