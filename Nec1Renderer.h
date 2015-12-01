//{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,1,^108m,(16,-4,1,^108m)*) [D:0..255,S:0..255=255-D,F:0..255]

#ifndef NEC1RENDERER_H
#define	NEC1RENDERER_H

#include <IrSignal.h>

class Nec1Renderer {
private:
    static const frequency_t frequency = 38400U;
    static const size_t introLength = 68U;
    static const size_t repeatLength = 4U;

public:
    static const IrSignal *newIrSignal(unsigned int D, unsigned int S, unsigned int F);
    static const IrSignal *newIrSignal(unsigned int D, unsigned int F) {
        return newIrSignal(D, 255-D, F);
    };

private:
    Nec1Renderer();
    static const microseconds_t repeatData[repeatLength];
    static const IrSequence repeat;
    static void lsbByte(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int D);
    static void process(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int data);
};

#endif	/* NEC1RENDERER_H */

