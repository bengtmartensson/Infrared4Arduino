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

// From IRLib.cpp, renamed from My_delay_uSecs.

//The Arduino built in function delayMicroseconds has limits we wish to exceed
//Therefore we have created this alternative
void IrSender::delayUSecs(microseconds_t T) {
    if (T) {
        if (T > 16000) {
            delayMicroseconds(T % 1000);
            delay(T / 1000);
        } else
            delayMicroseconds(T);
    };
}

IrSender::IrSender() {
    outputPin = invalidPin;
}

IrSender::IrSender(pin_t pin) {
    outputPin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

IrSender::~IrSender() {
    mute();
}

void IrSender::mute() {
    digitalWrite(outputPin, LOW);
}

void IrSender::sendIrSignal(const IrSignal& irSignal, unsigned int noSends) {
    if (!irSignal.getIntro().isEmpty())
        send(irSignal.getIntro(), irSignal.getFrequency());
    for (unsigned int i = 0; i < irSignal.noRepetitions(noSends); i++)
        send(irSignal.getRepeat(), irSignal.getFrequency());
    if (!irSignal.getEnding().isEmpty())
        send(irSignal.getEnding(), irSignal.getFrequency());
}
