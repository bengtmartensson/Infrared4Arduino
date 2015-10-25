#include "IrSignal.h"
#include "IrSender.h"

IrSignal::IrSignal(const IrSequence& intro_, const IrSequence& repeat_, const IrSequence& ending_, frequency_t frequency_)
: intro(&intro_),repeat(&repeat_),ending(&ending_),frequency(frequency_) {
}

IrSignal::IrSignal(const IrSignal& orig)
: intro(orig.intro),repeat(orig.repeat),ending(orig.ending),frequency(orig.frequency) {
}

IrSignal::IrSignal(const microseconds_t *intro_, unsigned int lengthIntro,
            const microseconds_t *repeat_, unsigned int lengthRepeat,
            const microseconds_t *ending_, unsigned int lengthEnding,
            frequency_t frequency_) : frequency(frequency_) {
    intro = new IrSequence(intro_, lengthIntro);
    repeat = new IrSequence(repeat_, lengthRepeat);
    ending = new IrSequence(ending_, lengthEnding);
}

#ifdef ARDUINO
void IrSignal::dump(Stream& stream) const {
    stream.print("f=");
    stream.println(frequency);
    intro.dump(stream);
    repeat.dump(stream);
    ending.dump(stream);
}
#endif
