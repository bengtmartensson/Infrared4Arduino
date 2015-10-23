#ifndef IRRENDERER_H
#define	IRRENDERER_H

#include "IrSignal.h"
#include "IrSender.h"

class IrRenderer {
private:
    IrSignal *irSignal;

protected:
    void setup(const IrSequence& intro, const IrSequence& repeat, const IrSequence& ending, frequency_t frequency) {
        irSignal = new IrSignal(intro, repeat, ending, frequency);
    }

    void setup(const IrSequence& intro, const IrSequence& repeat, frequency_t frequency) {
        setup(intro, repeat, *(new IrSequence()), frequency);
    }

    void setup(const microseconds_t introArray[], unsigned int introLength,
            const microseconds_t repeatArray[], unsigned int repeatLength,
            frequency_t frequency) {
        IrSequence intro(introArray, introLength);
        IrSequence repeat(repeatArray, repeatLength);
        setup(intro, repeat, frequency);
    }

public:
    IrRenderer() {
    };

    virtual ~IrRenderer() {
    };

    const IrSignal& render() const {
        return *irSignal;
    }

    /** Just a convenience function */
    void send(IrSender& irSender, unsigned int noSends = 1) {
        irSender.send(render(), noSends);
    }
};

#endif	/* IRRENDERER_H */

