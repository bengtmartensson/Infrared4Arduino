#include "IrSignal.h"
#include "IrSender.h"

IrSignal::IrSignal(const IrSequence& intro_, const IrSequence& repeat_, const IrSequence& ending_,
        frequency_t frequency_, bool toBeFreed)
: frequency(frequency_),intro(intro_, toBeFreed),repeat(repeat_, toBeFreed),ending(ending_, toBeFreed) {
}

IrSignal::IrSignal(const IrSequence& intro_, const IrSequence& repeat_, const IrSequence& ending_,
        frequency_t frequency_)
: frequency(frequency_),intro(intro_),repeat(repeat_),ending(ending_) {
}

//IrSignal::IrSignal(const IrSignal& orig, bool toBeFreed)
//: frequency(orig.frequency),intro(orig.intro,toBeFreed),repeat(orig.repeat,toBeFreed),ending(orig.ending,toBeFreed) {
//}

IrSignal::IrSignal(const IrSignal& orig)
: frequency(orig.frequency),intro(orig.intro),repeat(orig.repeat),ending(orig.ending) {
}

IrSignal::IrSignal(const microseconds_t *intro_, size_t introLength,
            const microseconds_t *repeat_, size_t repeatLength,
            const microseconds_t *ending_, size_t endingLength,
            frequency_t frequency_, bool toBeFreed)
: frequency(frequency_),
        intro(intro_, introLength, toBeFreed),
  repeat(repeat_, repeatLength, toBeFreed),
        ending(ending_, endingLength, toBeFreed) {
}

IrSignal::IrSignal(const microseconds_t *intro_, size_t introLength,
        const microseconds_t *repeat_, size_t repeatLength,
        frequency_t frequency_, bool toBeFreed)
: frequency(frequency_),
intro(intro_, introLength, toBeFreed),
repeat(repeat_, repeatLength, toBeFreed),
ending(NULL, 0, false) {
}

IrSignal::~IrSignal() {
    //delete intro;
    //delete repeat;
    //delete ending;
}

IrSignal *IrSignal::clone() const {
    return new IrSignal(*intro.clone(), *repeat.clone(), *ending.clone(), frequency, true);
}

void IrSignal::dump(Stream& stream, bool usingSigns) const {
    bool printedSomething = dumpFrequency(stream);
    if (printedSomething)
        stream.println();
    intro.dump(stream, usingSigns);
    repeat.dump(stream, usingSigns);
    ending.dump(stream, usingSigns);
}

bool IrSignal::dumpFrequency(Stream& stream, frequency_t frequency) {
    if (frequency > 0 && frequency != invalidFrequency) {
        stream.print(F("f="));
        stream.print(frequency);
        return true;
    }
    return false;
}
