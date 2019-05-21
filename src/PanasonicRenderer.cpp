#include "PanasonicRenderer.h"

// NOTE: writing intro[i++] = ... produces wrong result, compiler bug?
// (Adding a print statement immediately after, and it works :-~)
// So let's write intro[i] = ...; i++ at least for now.

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static const IrSequence emptyIrSequence;

// {37k,432}<1,-1|1,-3>(8,-4,2:8,32:8,D:8,S:8,F:8,(D^S^F):8,1,-173)* [D:0..255,S:0..255,F:0..255]

const IrSignal *PanasonicRenderer::newIrSignal(unsigned int D, unsigned int S, unsigned int F) {
    microseconds_t *data = new microseconds_t[dataLength];
    unsigned int i = 0U;
    uint32_t sum = (8U + 4U)*timebase;
    data[i] = 8U * timebase; i++;
    data[i] = 4U * timebase; i++;
    lsbByte(data, i, sum, 2U);
    lsbByte(data, i, sum, 32U);
    lsbByte(data, i, sum, D);
    lsbByte(data, i, sum, S);
    lsbByte(data, i, sum, F);
    lsbByte(data, i, sum, D ^ S ^ F);
    data[i] = timebase; i++;
    data[i] = (microseconds_t) MIN(173U*timebase, MICROSECONDS_T_MAX); i++;
    IrSequence *repeatPtr = new IrSequence(data, dataLength, true);
    return new IrSignal(emptyIrSequence, *repeatPtr, emptyIrSequence, frequency);
}

void PanasonicRenderer::lsbByte(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int X) {
    for (unsigned int index = 0; index < 8U; index++) {
        transmitBit(intro, i, sum, X & 1U);
        X >>= 1U;
    }
}

void inline PanasonicRenderer::transmitBit(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int data) {
    intro[i++] = timebase;
    intro[i++] = (data ? 3U : 1U)*timebase;
    sum += (data ? 4U : 2U)*timebase;
}
