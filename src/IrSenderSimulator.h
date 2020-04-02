#pragma once

#include "IrSender.h"

/**
 * Simulates sending in the sense that it prints the IrSequences on the Stream
 * given as argument. Intended as debugging and development tool.
 */
class IrSenderSimulator : public IrSender {
private:
    Stream& stream;

public:
    IrSenderSimulator(Stream& stream_) : IrSender(Board::NO_PIN), stream(stream_) {};
    IrSenderSimulator(const IrSenderSimulator& orig) : IrSender(Board::NO_PIN),stream(orig.stream) {};
    virtual ~IrSenderSimulator() {};
    void send(const IrSequence& irSequence, frequency_t frequency = IrSignal::defaultFrequency, dutycycle_t dutyCycle = Board::defaultDutyCycle);
    void enable(frequency_t, dutycycle_t d __attribute__((unused)) = Board::defaultDutyCycle ) {};
    void sendSpace(microseconds_t) {};
    void sendMark(microseconds_t) {};
};
