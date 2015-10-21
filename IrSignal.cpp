#include "IrSignal.h"

IrSignal::IrSignal() {
}

IrSignal::~IrSignal() {
}

IrSignal::IrSignal(frequency_t frequency_, unsigned int lengthIntro_, unsigned int lengthRepeat_, unsigned int lengthEnding_,
        const microseconds_t *intro_, const microseconds_t *repeat_, const microseconds_t *ending_) {
    frequency = frequency_;
    introLength = lengthIntro_;
    repeatLength = lengthRepeat_;
    endingLength = lengthEnding_;
    intro = intro_;
    repeat = repeat_;
    ending = ending_;
}

void IrSignal::dump(Stream& stream) {
    stream.print("f=");
    stream.println(frequency);
    dump(stream, introLength, intro);
    dump(stream, repeatLength, repeat);
    dump(stream, endingLength, ending);
}

void IrSignal::dump(Stream& stream, unsigned int length, const microseconds_t *data) {
    for (unsigned int i = 0U; i < length; i++) {
        if (i > 0U)
            stream.print(' ');
        stream.print((i & 1) ? '-' : '+');
        stream.print(data[i], DEC);
    }
    stream.println();
}
