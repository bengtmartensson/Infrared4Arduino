// {36k,msb,889}<1,-1|-1,1>((1:1,~F:1:6,T:1,D:5,F:6,^114m)+,T=1-T)[T@:0..1=0,D:0..31,F:0..127]

#include "Rc5Renderer.h"

uint8_t Rc5Renderer::T = 1;

Rc5Renderer::~Rc5Renderer() {
}

Rc5Renderer::Rc5Renderer(uint16_t D, uint16_t F) {
    T = ! T;
    init(D, F, T);
}

void Rc5Renderer::init(unsigned int D, unsigned int F, unsigned int T) {
    index = 0;
    pending = 0;
    emit(1U);
    emit(((~F) & 0x40U) >> 6U);
    emit(T & 1U);
    emitMsb(D, 5U);
    emitMsb(F, 6U);
    emitEnd();
    setup(NULL, 0, repeat, index, frequency);
}

//const IrSignal& Rc5Renderer::render() const {
//    return *(new IrSignal(frequency, 0, index, 0, NULL, repeat, NULL));
//}

void Rc5Renderer::emitMsb(unsigned int x, unsigned int length) {
    unsigned int mask = 1U << (length - 1U);
    while (mask != 0U) {
        emit((x & mask) != 0);
        mask >>= 1U;
    }
}

void Rc5Renderer::emit(unsigned int x) {
    if (pending == 0) {
        // First, do nothing, just stuff in pending.
    } else if ((pending > 0) == ((x & 1) != 0)) {
        repeat[index++] = timebase;
        repeat[index++] = timebase;
    } else {
        repeat[index++] = 2 * timebase;
    }
    pending = (x & 1U) ? 1 : -1;
}

void Rc5Renderer::emitEnd() {
    if (pending > 0)
        repeat[index++] = timebase;

    repeat[index++] = 0xFFFF;
}
