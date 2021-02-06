#include "IrSenderSimulator.h"

void IrSenderSimulator::send(const IrSequence& irSequence, frequency_t frequency, dutycycle_t dutyCycle) {
    if (! irSequence)
        return;

    stream.print(F("IrSenderSimulator: "));
    bool printedSomething = IrSignal::dumpFrequency(stream, frequency);
    if (printedSomething)
        stream.print(' ');
    printedSomething = IrSignal::dumpDutyCycle(stream, dutyCycle);
    if (printedSomething)
        stream.print(' ');
    irSequence.dump(stream, true);
}
