#include "Rc5Renderer.h"

// NOTE: writing intro[i++] = ... produces wrong result, compiler bug?
// (Adding a print statement immediately after, and it works :-~)
// So let's write intro[i] = ...; i++ at least for now.

#define MIN(a, b) ((a) < (b) ? (a) : (b))

uint8_t Rc5Renderer::T = 1U;

const IrSignal *Rc5Renderer::newIrSignal(unsigned int D, unsigned int F) {
    T = ! T;
    return newIrSignal(D, F, T);
}

static const IrSequence emptyIrSequence;

const IrSignal *Rc5Renderer::newIrSignal(unsigned int D, unsigned int F, unsigned int T) {
    unsigned int index = 0U;
    int pending = 0;
    microseconds_t *repeat = new microseconds_t[28];
    emit(1U, index, pending, repeat);
    emit(((~F) & 0x40U) >> 6U, index, pending, repeat);
    emit(T & 1U, index, pending, repeat);
    emitMsb(D, 5U, index, pending, repeat);
    emitMsb(F, 6U, index, pending, repeat);
    emitEnd(index, pending, repeat);
    return new IrSignal(nullptr, 0, repeat, index, frequency);
}

void Rc5Renderer::emitMsb(unsigned int x, unsigned int length,
        unsigned int& index, int& pending, microseconds_t *repeat) {
    unsigned int mask = 1U << (length - 1U);
    while (mask != 0U) {
        emit((x & mask) != 0, index, pending, repeat);
        mask >>= 1U;
    }
}

void Rc5Renderer::emit(unsigned int x, unsigned int& index, int& pending,
                       microseconds_t *repeat) {
    if (pending == 0) {
        // First, do nothing, just stuff in pending.
    } else if ((pending > 0) == ((x & 1U) != 0U)) {
        repeat[index] = timebase;
        index++;
        repeat[index] = timebase;
        index++;
    } else {
        repeat[index] = 2U * timebase;
        index++;
    }
    pending = (x & 1U) ? 1 : -1;
}

void Rc5Renderer::emitEnd(unsigned int& index, int& pending, microseconds_t *repeat) {
    if (pending > 0) {
        repeat[index] = timebase; index++;
    }
    repeat[index] = MIN(90000U, MICROSECONDS_T_MAX); index++;
}
