#ifndef ARDUINO
#include "Arduino.h"

uint8_t currentWritePin = 255;
struct timeval simulatedTime = getTimeOfDay();

#endif