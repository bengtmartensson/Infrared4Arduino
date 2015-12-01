#include "Nec1Renderer.h"

// NOTE: writing intro[i++] = ... produces wrong result, compiler bug?
// (Adding a print statement immediately after, and it works :-~)
// So let's write intro[i] = ...; i++ at least for now.

const microseconds_t Nec1Renderer::repeatData[repeatLength] = { 9024, 2256, 564, 65535 };
const IrSequence Nec1Renderer::repeat(repeatData, repeatLength, false);

const IrSignal *Nec1Renderer::newIrSignal(unsigned int D, unsigned int S, unsigned int F) {
    microseconds_t *introData = new microseconds_t[introLength];
    unsigned int i = 0;
    uint32_t sum = 9024 + 4512 + 564;
    introData[i] = 9024; i++;
    introData[i] = 4512; i++;
    lsbByte(introData, i, sum, D);
    lsbByte(introData, i, sum, S);
    lsbByte(introData, i, sum, F);
    lsbByte(introData, i, sum, 255-F);
    introData[i] = 564; i++;
    introData[i] = (microseconds_t) (108000 - sum); i++;
    IrSequence *intro = new IrSequence(introData, introLength, true);
    IrSequence *ending = new IrSequence();
    return new IrSignal(*intro, repeat, *ending, frequency);
}

void Nec1Renderer::lsbByte(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int X) {
    for (unsigned int index = 0; index < 8; index++) {
        process(intro, i, sum, X & 1);
        X >>= 1;
    }
}

void inline Nec1Renderer::process(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int data) {
    intro[i++] = 564;
    intro[i++] = data ? 1692 : 564;
    sum += data ? 564+1692 : 564+564;
}
