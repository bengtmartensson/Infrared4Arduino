/*
Copyright (C) 2015 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#include "IrSender.h"
#include "IrSignal.h"

IrSender::IrSender(pin_t pin) : sendPin(pin) {
    Board::getInstance()->setPinMode(pin, OUTPUT);
    Board::getInstance()->writeLow(pin);
}

IrSender::~IrSender() {
    mute();
}

void IrSender::sendIrSignal(const IrSignal& irSignal, unsigned int noSends) {
    send(irSignal.getIntro(), irSignal.getFrequency());
    for (unsigned int i = 0; i < irSignal.noRepetitions(noSends); i++)
        send(irSignal.getRepeat(), irSignal.getFrequency());
    send(irSignal.getEnding(), irSignal.getFrequency());
}

void IrSender::sendWhile(const IrSignal& irSignal, bool(*trigger)()) {
    if (trigger()) {
        // "Button is pressed", send

        // Send the intro sequence,...
        sendIrSignal(irSignal, 1);

        // ... then, for as long as the button is held,
        // send the repeat sequence
        while (trigger()) {
            send(irSignal.getRepeat());
        }

        // finally, the ending sequence (normally empty)
        send(irSignal.getEnding());
    } else {
        // Button is not pressed, do nothing.
    }
}

void IrSender::send(const IrSequence& irSequence, frequency_t frequency, dutycycle_t dutyCycle) {
    enable(frequency, dutyCycle);
#ifdef CONSIDER_COMPUTATIONAL_DELAYS
    uint32_t refTime = micros();
#endif
    for (unsigned int i = 0U; i < irSequence.getLength(); i++) {
#ifdef CONSIDER_COMPUTATIONAL_DELAYS
#error not tested
        microseconds_t duration = irSequence[i];
        refTime += duration;
        int32_t delay = refTime - micros(); // TODO verify overflow
        if (delay <= 0)
            return;
#else
        microseconds_t delay = irSequence[i];
#endif
        if (i & 1U)
            sendSpace(delay);
        else
            sendMark(delay);
    }
}
