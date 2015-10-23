#ifndef IRRENDERER_H
#define	IRRENDERER_H

#include "IrSignal.h"
#include "IrSender.h"

class IrRenderer {
public:
    IrRenderer() {
    };

    virtual ~IrRenderer() {
    };

    virtual const IrSignal& render() const = 0;

    /** Just a convenience function */
    void send(IrSender& irSender, unsigned int noSends = 1) {
        irSender.send(render(), noSends);
    }

    /** Just a convenience function */
    void send(IrSender *irSender, unsigned int noSends = 1) {
        irSender->send(render(), noSends);
    }
};

#endif	/* IRRENDERER_H */

