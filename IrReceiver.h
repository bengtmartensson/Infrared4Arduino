#ifndef IRRECEIVER_H
#define	IRRECEIVER_H

#include <Arduino.h>
#include "InfraredTypes.h"
#include "IrReader.h"

/**
 * This class servers as an abstract base class for (demodulating) IR receivers.
 */
class IrReceiver : public IrReader {
private:
    /** GPIO pin the receiver is using. */
    pin_t pin;
protected:
    /** Microseconds subtracted from pulses and added to gaps. */
    microseconds_t markExcess;



public:
    // Default values
    static const pin_t defaultPin = 5;
    static const microseconds_t defaultMarkExcess = 50U;

    /** Using inverting sensor, like most TSOPs? */
    static const boolean invertingSensor = true;
    /**
     * Constructor.
     *
     * @param bufSize
     * @param pin GPIO pin to be used. Will be setup by the constructor.
     * @param pullup Enable hardware pullup within the processor.
     * @param markExcess
     */
    IrReceiver(unsigned int bufSize, pin_t pin, boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess);

    virtual ~IrReceiver() {
    };
    virtual void enable() = 0;
    virtual void disable() = 0;

    pin_t getPin() const {
        return pin;
    }

    void setMarkExcess(microseconds_t markExcess_) {
        markExcess = markExcess_;
    }

    microseconds_t getMarkExcess() const {
        return markExcess;
    }

    enum irdata_t {
        IR_MARK,
        IR_SPACE
    };

    irdata_t readIr() {
        return ((digitalRead(getPin()) == HIGH) ^ invertingSensor) ? IR_MARK : IR_SPACE;
    }
};

#endif	/* IRRECEIVER_H */
