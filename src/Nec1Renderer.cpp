#include "Nec1Renderer.h"

// NOTE: writing intro[i++] = ... produces wrong result, compiler bug?
// (Adding a print statement immediately after, and it works :-~)
// So let's write intro[i] = ...; i++ at least for now.

#define MIN(a, b) ((a) < (b) ? (a) : (b))

const microseconds_t Nec1Renderer::repeatData[repeatLength] = { 9024, 2256, 564, MIN(96156, MICROSECONDS_T_MAX) };
const IrSequence Nec1Renderer::repeat(repeatData, repeatLength, false);
static const IrSequence emptyIrSequence;

const IrSignal *Nec1Renderer::newIrSignal(unsigned int D, unsigned int S, unsigned int F) {
    microseconds_t *introData = new microseconds_t[introLength];
    unsigned int i = 0U;
    uint32_t sum = 9024U + 4512U + 564U;
    introData[i] = 9024U; i++;
    introData[i] = 4512U; i++;
    lsbByte(introData, i, sum, D);
    lsbByte(introData, i, sum, S);
    lsbByte(introData, i, sum, F);
    lsbByte(introData, i, sum, 255U-F);
    introData[i] = 564U; i++;
    introData[i] = (microseconds_t) (108000U - sum); i++;
    IrSequence *introPtr = new IrSequence(introData, introLength, true);
    return new IrSignal(*introPtr, repeat, emptyIrSequence, frequency);
}

void Nec1Renderer::lsbByte(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int X) {
    for (unsigned int index = 0; index < 8U; index++) {
        process(intro, i, sum, X & 1U);
        X >>= 1U;
    }
}

void inline Nec1Renderer::process(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int data) {
    intro[i++] = 564U;
    intro[i++] = data ? 1692U : 564U;
    sum += data ? 564U+1692U : 564U+564U;
}
