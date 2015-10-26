#ifndef IRSENDERSIMULATOR_H
#define	IRSENDERSIMULATOR_H

#include "IrSender.h"

/**
 * Simulates sending in the sense that it prints the IrSequences on the Stream
 * given as argument. Intended as debugging and development tool.
 */
class IrSenderSimulator : public IrSender {
private:
    Stream stream;

public:
    IrSenderSimulator(Stream& stream_) : stream(stream_) {};
    IrSenderSimulator(const IrSenderSimulator& orig) : stream(orig.stream) {};
    virtual ~IrSenderSimulator() {};
    void send(const IrSequence&, frequency_t);
};

#endif	/* IRSENDERSIMULATOR_H */
