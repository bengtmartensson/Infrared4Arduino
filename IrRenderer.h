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
        irSignal = new IrSignal(introArray, introLength, repeatArray, repeatLength, NULL, 0, frequency);
    }

public:
    IrRenderer() {
    };

    virtual ~IrRenderer() {
        delete irSignal;
    };

    const IrSignal& render() const {
        return *irSignal;
    };

    //operator const IrSignal& () const {
    //    return *irSignal;
    //}

    /** Just a convenience function */
    void send(IrSender& irSender, unsigned int noSends = 1) {
        irSender.sendSignal(render(), noSends);
    }

    virtual const char *getProtocolName() const = 0;
};

#endif	/* IRRENDERER_H */

