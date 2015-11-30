#ifdef ARDUINO
#error This file is not intended to be run on the Arduino.
#endif

#include "Arduino.h"
#include "Nec1Renderer.h"
#include "IrSenderPwm.h"
#include "IrSequenceReader.h"
#include "Nec1Decoder.h"
#include "IrSenderSimulator.h"
#include "Pronto.h"
#include <unistd.h>
#include <iostream>

int main() {
    IrRenderer *renderer;
    renderer = new Nec1Renderer(122, 29); // power_on for Yahama receivers
    Stream str(std::cout);
    renderer->render().dump(str, true);
    //const IrSignal& irSignal = (const IrSignal&)renderer;
    //irSignal.dump(str);

    IrSequenceReader irSequenceReader(renderer->render().getIntro());
    Nec1Decoder decoder(irSequenceReader);
    decoder.printDecode(str);

    IrSenderSimulator sender(str);
    renderer->send(sender, 3);
    //sender->sendSignal(*renderer, 3);

    IrSignal *sig = Pronto::parse("0000 006C 0022 0002 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 015B 0057 0016 0E6C");
    if (sig == NULL)
        std::cerr << "Blast!" << std::endl;
    else
        sig->dump(str, true);
}
