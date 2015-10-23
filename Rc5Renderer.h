// {36k,msb,889}<1,-1|-1,1>((1:1,~F:1:6,T:1,D:5,F:6,^114m)+,T=1-T)[T@:0..1=0,D:0..31,F:0..127]

#ifndef RC5RENDERER_H
#define	RC5RENDERER_H

#include <Arduino.h>
#include "IrRenderer.h"

class Rc5Renderer : public IrRenderer {
public:
    static const frequency_t frequency = 36000;
    static const unsigned int introLength = 0;
    static const unsigned int endingLength = 0;

    Rc5Renderer(unsigned int D, unsigned int F, unsigned int T) { init(D, F, T); }

    /** This function uses an internal toggle of the class to compute T */
    Rc5Renderer(unsigned int D, unsigned int F);

    virtual ~Rc5Renderer();

    const IrSignal& render() const;

private:
    unsigned int index;
    microseconds_t repeat[28];
    int pending;
    static const microseconds_t timebase = 889;
    void emit(unsigned int t);
    void emitMsb(unsigned int x, unsigned int length);
    void emitEnd();
    void init(unsigned int D, unsigned int F, unsigned int T);

    static uint8_t T;
};

#endif	/* RC5RENDERER_H */

