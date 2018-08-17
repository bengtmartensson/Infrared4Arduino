#ifndef IRRECEIVER_H
#define	IRRECEIVER_H

#include <Arduino.h>
#include "InfraredTypes.h"
#include "IrReader.h"
#include "IrSignal.h"

/**
 * Abstract base class for demodulating IR receivers.
 */
class IrReceiver : public IrReader {
private:
    /** GPIO pin the receiver is connected to. */
    pin_t pin;

public:
    // Default values
    static const pin_t defaultPin = 5;
    static const microseconds_t defaultMarkExcess = 50U;

    /** Are we using inverting sensor, like most TSOPs? */
    static const bool invertingSensor = true;

    /**
     * Constructor.
     *
     * @param bufSize
     * @param pin GPIO pin to be used. Will be setup by the constructor.
     * @param pullup Enable hardware pullup within the processor.
     * @param markExcess
     */
    IrReceiver(size_t bufSize, pin_t pin, bool pullup = false,
            microseconds_t markExcess = defaultMarkExcess);

    virtual ~IrReceiver() {
    };

    virtual frequency_t getFrequency() const {
        return IrSignal::defaultFrequency;
    };

    virtual void receive();

    pin_t getPin() const {
        return pin;
    }

    /**
     * Enum for the duration types.
     */
    enum irdata_t {
        IR_MARK, ///< on-period, also called flash
        IR_SPACE ///< off-period, also called gap
    };

    // Needs to be public since used in ISP. Therefore hide it from Doxygen
    /// @cond false
    irdata_t readIr() {
        return ((digitalRead(getPin()) == HIGH) ^ invertingSensor) ? IR_MARK : IR_SPACE;
    }
    /// @endcond
};

#endif	/* IRRECEIVER_H */
