#include "IrSignal.h"
#include "IrSender.h"

IrSignal::IrSignal(const IrSequence& intro_, const IrSequence& repeat_, const IrSequence& ending_,
        frequency_t frequency_, dutycycle_t dutyCycle_)
: frequency(frequency_),dutyCycle(dutyCycle_),intro(intro_),repeat(repeat_),ending(ending_) {
}

IrSignal::IrSignal(IrSequence&& intro_, IrSequence&& repeat_, IrSequence&& ending_,
        frequency_t frequency_, dutycycle_t dutyCycle_)
: frequency(frequency_),dutyCycle(dutyCycle_),intro(intro_),repeat(repeat_),ending(ending_) {
}

IrSignal::IrSignal(const IrSequence& intro_, const IrSequence& repeat_,
        frequency_t frequency_, dutycycle_t dutyCycle_)
: frequency(frequency_), dutyCycle(dutyCycle_), intro(intro_), repeat(repeat_), ending() {
}

IrSignal::IrSignal(IrSequence&& intro_, IrSequence&& repeat_,
        frequency_t frequency_, dutycycle_t dutyCycle_)
: frequency(frequency_), dutyCycle(dutyCycle_), intro(intro_), repeat(repeat_), ending() {
}

IrSignal::IrSignal(const microseconds_t *intro_, size_t introLength,
        const microseconds_t *repeat_, size_t repeatLength,
        const microseconds_t *ending_, size_t endingLength,
        frequency_t frequency_, dutycycle_t dutyCycle_)
: frequency(frequency_),
dutyCycle(dutyCycle_),
intro(intro_, introLength),
repeat(repeat_, repeatLength),
ending(ending_, endingLength) {
}

IrSignal::IrSignal(const microseconds_t *intro_, size_t introLength,
        const microseconds_t *repeat_, size_t repeatLength,
        frequency_t frequency_, dutycycle_t dutyCycle_)
: frequency(frequency_),
dutyCycle(dutyCycle_),
intro(intro_, introLength),
repeat(repeat_, repeatLength),
ending() {
}

#if HAS_FLASH_READ

IrSignal* IrSignal::readFlash(const microseconds_t *intro, size_t lengthIntro,
        const microseconds_t *repeat, size_t lengthRepeat,
        const microseconds_t *ending, size_t lengthEnding,
        frequency_t frequency_,
        dutycycle_t dutyCycle_) {
    return new IrSignal(*IrSequence::readFlash(intro, lengthIntro),
            *IrSequence::readFlash(repeat, lengthRepeat),
            *IrSequence::readFlash(ending, lengthEnding),
            frequency_, dutyCycle_);
}

IrSignal* IrSignal::readFlash(const microseconds_t *intro, size_t lengthIntro,
        const microseconds_t *repeat, size_t lengthRepeat,
        frequency_t frequency_,
        dutycycle_t dutyCycle_) {
    return new IrSignal(*IrSequence::readFlash(intro, lengthIntro),
            *IrSequence::readFlash(repeat, lengthRepeat),
             frequency_, dutyCycle_);
}
#endif

void IrSignal::dump(Stream& stream, bool usingSigns) const {
    bool printedSomething = dumpFrequency(stream);
    if (printedSomething)
        stream.print(" ");
    printedSomething = dumpDutyCycle(stream);
    if (printedSomething)
        stream.print(" ");
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

bool IrSignal::dumpDutyCycle(Stream& stream, dutycycle_t dutyCycle) {
  if (dutyCycle > 0 && dutyCycle < 100) {
        stream.print(dutyCycle);
        stream.print("%");
        return true;
    }
    return false;
}