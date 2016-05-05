#ifndef IRSENDERSIMULATOR_H
#define	IRSENDERSIMULATOR_H

#include "IrSender.h"

/**
 * Simulates sending in the sense that it prints the IrSequences on the Stream
 * given as argument. Intended as debugging and development tool.
 */
class IrSenderSimulator : public IrSender {
private:
    Stream& stream;

public:
    IrSenderSimulator(Stream& stream_) : stream(stream_) {};
    IrSenderSimulator(const IrSenderSimulator& orig) : IrSender(),stream(orig.stream) {};
    virtual ~IrSenderSimulator() {};
    void send(const IrSequence& irSequence, frequency_t frequency = IrSignal::defaultFrequency);
};

#endif	/* IRSENDERSIMULATOR_H */
