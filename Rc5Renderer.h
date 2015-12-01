// {36k,msb,889}<1,-1|-1,1>((1:1,~F:1:6,T:1,D:5,F:6,^114m)+,T=1-T)[T@:0..1=0,D:0..31,F:0..127]

#ifndef RC5RENDERER_H
#define	RC5RENDERER_H

#include "IrSignal.h"

class Rc5Renderer {
private:
    static const frequency_t frequency = 36000U;
    static const size_t introLength = 0U;
    static const size_t endingLength = 0U;

public:
    static const IrSignal *newIrSignal(unsigned int D, unsigned int F, unsigned int T);

    /** This function uses an internal toggle of the class to compute T */
    static const IrSignal *newIrSignal(unsigned int D, unsigned int F);

private:
    Rc5Renderer();
    //unsigned int index;
    //microseconds_t repeat[28];
    //int pending;
    static const microseconds_t timebase = 889;
    static void emit(unsigned int t, unsigned int& index, int& pending, microseconds_t *repeat);
    static void emitMsb(unsigned int x, unsigned int length, unsigned int& index,
                        int& pending, microseconds_t *repeat);
    static void emitEnd(unsigned int& index, int& pending, microseconds_t *repeat);

    static uint8_t T;
};

#endif	/* RC5RENDERER_H */

