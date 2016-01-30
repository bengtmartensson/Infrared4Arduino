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
    /** GPIO pin the receiver is connected to. */
    pin_t pin;

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
    IrReceiver(size_t bufSize, pin_t pin, boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess);

    virtual ~IrReceiver() {
    };
    virtual void receive();

    pin_t getPin() const {
        return pin;
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
