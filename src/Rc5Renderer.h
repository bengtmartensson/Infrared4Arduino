#pragma once

#include "IrSignal.h"
/**
 * A static class consisting of two functions that generate IrSignal-s from the RC5 protocol parameters.
 * The RC5 protocol is given in IRP notation as
 * \c {36k,msb,889}<1,-1|-1,1>((1:1,~F:1:6,T:1,D:5,F:6,^114m)+,T=1-T)[T\@:0..1=0,D:0..31,F:0..127]
 */
class Rc5Renderer {
private:
    static constexpr frequency_t frequency = 36000UL;
    static constexpr size_t introLength = 0U;
    static constexpr size_t endingLength = 0U;

public:
    /**
     * Generates an RC5 signal from the RC5 parameters.
     * Must be deleted explicitly by the user to avoid memory leaks.
     * @param D RC5 parameter, "device"
     * @param F RC5 parameter, "function"
     * @param T RC5 parameter, "toggle"
     * @return IrSignal
     */
    static const IrSignal *newIrSignal(unsigned int D, unsigned int F, unsigned int T);

    /**
     * Generates an RC5 signal from the RC5 parametes.
     * This version uses an internal toggle of the class to compute T.
     * Must be deleted explicitly by the user to avoid memory leaks.
     * @param D RC5 parameter, "device"
     * @param F RC5 parameter, "function"
     * @return IrSignal
     */
    static const IrSignal *newIrSignal(unsigned int D, unsigned int F);

private:
    Rc5Renderer();
    static constexpr microseconds_t timebase = 889U;
    static void emit(unsigned int t, unsigned int& index, int& pending, microseconds_t *repeat);
    static void emitMsb(unsigned int x, unsigned int length, unsigned int& index,
                        int& pending, microseconds_t *repeat);
    static void emitEnd(unsigned int& index, int& pending, microseconds_t *repeat);

    static uint8_t T;
};
