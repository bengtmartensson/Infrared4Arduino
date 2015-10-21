#include "IrSignal.h"

IrSignal::IrSignal() {
}

IrSignal::~IrSignal() {
}

IrSignal::IrSignal(frequency_t frequency_, uint16_t lengthIntro_, uint16_t lengthRepeat_, uint16_t lengthEnding_,
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

void IrSignal::dump(Stream& stream, uint16_t length, const microseconds_t *data) {
    for (uint16_t i = 0U; i < length; i++) {
        if (i > 0U)
            stream.print(' ');
        stream.print((i & 1) ? '-' : '+');
        stream.print(data[i]);
    }
    stream.println();
}
