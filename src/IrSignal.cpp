#include "IrSignal.h"
#include "IrSender.h"

IrSignal::IrSignal(IrSequence& intro, IrSequence& repeat, IrSequence& ending,
        frequency_t frequency, boolean toBeFreed)
: frequency(frequency), intro(intro, toBeFreed), repeat(repeat, toBeFreed), ending(ending, toBeFreed) {
}

IrSignal::IrSignal(const IrSequence& intro, const IrSequence& repeat, const IrSequence& ending,
        frequency_t frequency, boolean toBeFreed __attribute__((unused)))
: frequency(frequency), intro(intro, false), repeat(repeat, false), ending(ending, false) {
}

IrSignal::IrSignal(IrSignal& orig, boolean toBeFreed)
: IrSignal(orig.intro, orig.repeat, orig.ending, orig.frequency, toBeFreed) {
}

IrSignal::IrSignal(const IrSignal& orig, boolean toBeFreed __attribute__((unused)))
: IrSignal(orig.intro, orig.repeat, orig.ending, orig.frequency, false) {
}

IrSignal::IrSignal(const microseconds_t *intro_, size_t introLength,
            const microseconds_t *repeat_, size_t repeatLength,
            const microseconds_t *ending_, size_t endingLength,
            frequency_t frequency_, boolean toBeFreed)
: frequency(frequency_),
        intro(intro_, introLength, toBeFreed),
  repeat(repeat_, repeatLength, toBeFreed),
        ending(ending_, endingLength, toBeFreed) {
}

IrSignal *IrSignal::clone() const {
    return new IrSignal(*intro.clone(), *repeat.clone(), *ending.clone(), frequency, true);
}

void IrSignal::dump(Stream& stream, boolean usingSigns) const {
    boolean printedSomething = dumpFrequency(stream);
    if (printedSomething)
        stream.println();
    intro.dump(stream, usingSigns);
    repeat.dump(stream, usingSigns);
    ending.dump(stream, usingSigns);
}

boolean IrSignal::dumpFrequency(Stream& stream, frequency_t frequency) {
    if (frequency > 0 && frequency != invalidFrequency) {
        stream.print(F("f="));
        stream.print(frequency);
        return true;
    }
    return false;
}
