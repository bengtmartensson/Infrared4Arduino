#ifdef ARDUINO
#error This file is not intended to be run on the Arduino.
#endif

#include "Arduino.h"
#include "Nec1Renderer.h"
#include "IrSenderPwm.h"
#include "IrSequenceReader.h"
#include "Nec1Decoder.h"
#include "IrSenderSimulator.h"
#include <unistd.h>
#include <iostream>

int main() {
    IrRenderer *renderer;
    renderer = new Nec1Renderer(122, 29); // power_on for Yahama receivers
    Stream str(std::cout);
    renderer->render().dump(str, true);

    IrSequenceReader irSequenceReader(renderer->render().getIntro());
    Nec1Decoder decoder(irSequenceReader);
    decoder.printDecode(str);

    IrSender *sender = new IrSenderSimulator(str);
    sender->sendSignal(*renderer, 3);
}
