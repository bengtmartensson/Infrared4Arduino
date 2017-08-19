#include <Arduino.h>
#include "IrReceiverPoll.h"

IrReceiverPoll::IrReceiverPoll(size_t captureLength,
        pin_t pin_,
        bool pullup,
        microseconds_t markExcess,
        milliseconds_t beginningTimeout,
        milliseconds_t endingTimeout) : IrReceiver(captureLength, pin_, pullup, markExcess) {
    setBeginningTimeout(beginningTimeout);
    setEndingTimeout(endingTimeout);
    durationData = new microseconds_t[bufferSize];
    dataLength = 0;
}

IrReceiverPoll::~IrReceiverPoll() {
    delete [] durationData;
    // let the pin stay as input
}

void IrReceiverPoll::reset() {
    IrReader::reset();
    dataLength = 0;
}

void IrReceiverPoll::enable() {
    reset();
    timeouted = !searchForStart();
    if (timeouted)
        return;
    collectData();
}

unsigned long timeSince(unsigned long then) {
    return micros() - then;
}

bool IrReceiverPoll::searchForStart() {
    unsigned long start = micros();
    unsigned long beginningTimeoutInMicros = 1000UL * beginningTimeout;
    while (readIr() == IrReceiver::IR_SPACE)
        if (timeSince(start) > beginningTimeoutInMicros)
            return false;
    return true;
}

void IrReceiverPoll::collectData() {
    IrReceiver::irdata_t lastDataRead = IrReceiver::IR_MARK;
    unsigned long endingTimeoutInMicros = 1000UL * endingTimeout;
    unsigned long lastTime = micros();
    while (dataLength < bufferSize) {
        unsigned long now = micros();
        IrReceiver::irdata_t data = readIr();
        if (data != lastDataRead) {
            recordDuration(now - lastTime);
            lastDataRead = data;
            lastTime = now;
        } else if (data == HIGH && (now - lastTime > endingTimeoutInMicros)) {
            recordDuration(now - lastTime);
            return; // normal exit
        }
    }
}

void IrReceiverPoll::recordDuration(unsigned long t) {
    durationData[dataLength++] = t <= MICROSECONDS_T_MAX ? (microseconds_t) t : MICROSECONDS_T_MAX;
}
