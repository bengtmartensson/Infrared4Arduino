#include "IrSignal.h"

IrSignal::IrSignal(const IrSequence& intro_, const IrSequence& repeat_, const IrSequence& ending_, frequency_t frequency_)
: intro(intro_),repeat(repeat),ending(ending_),frequency(frequency_) {
    //intro = intro_;
    //repeat = repeat_;
    //ending = ending_;
}

void IrSignal::dump(Stream& stream) const {
    stream.print("f=");
    stream.println(frequency);
    intro.dump(stream);
    repeat.dump(stream);
    ending.dump(stream);
}
