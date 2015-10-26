#include "IrSignal.h"
#include "IrSender.h"

IrSignal::IrSignal(const IrSequence& intro_, const IrSequence& repeat_, const IrSequence& ending_, frequency_t frequency_)
: frequency(frequency_),intro(intro_),repeat(repeat_),ending(ending_) {
}

IrSignal::IrSignal(const IrSignal& orig)
: frequency(orig.frequency),intro(orig.intro),repeat(orig.repeat),ending(orig.ending) {
}

IrSignal::IrSignal(const microseconds_t *intro_, unsigned int introLength,
            const microseconds_t *repeat_, unsigned int repeatLength,
            const microseconds_t *ending_, unsigned int endingLength,
            frequency_t frequency_)
: frequency(frequency_),
        intro(intro_, introLength),
  repeat(repeat_, repeatLength),
        ending(ending_, endingLength) {
}

void IrSignal::dump(Stream& stream, boolean usingSigns) const {
    stream.print("f=");
    stream.println(frequency);
    intro.dump(stream, usingSigns);
    repeat.dump(stream, usingSigns);
    ending.dump(stream, usingSigns);
}
