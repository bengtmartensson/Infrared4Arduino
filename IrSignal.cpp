#include "IrSignal.h"
#include "IrSender.h"

IrSignal::IrSignal(const IrSequence& intro_, const IrSequence& repeat_, const IrSequence& ending_, frequency_t frequency_)
: frequency(frequency_),intro(intro_),repeat(repeat_),ending(ending_) {
}

IrSignal::IrSignal(const IrSignal& orig)
: frequency(orig.frequency),intro(orig.intro),repeat(orig.repeat),ending(orig.ending) {
}

IrSignal::IrSignal(const microseconds_t *intro_, size_t introLength,
            const microseconds_t *repeat_, size_t repeatLength,
            const microseconds_t *ending_, size_t endingLength,
            frequency_t frequency_)
: frequency(frequency_),
        intro(intro_, introLength),
  repeat(repeat_, repeatLength),
        ending(ending_, endingLength) {
}

IrSignal::~IrSignal() {
    delete intro;
    delete repeat;
    delete ending;
}

IrSignal *IrSignal::clone() const {
    return new IrSignal(*intro.clone(), *repeat.clone(), *ending.clone(), frequency);
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
