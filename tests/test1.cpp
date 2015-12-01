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
#include "Rc5Renderer.h"
#include "Rc5Decoder.h"
#include <unistd.h>
#include <iostream>

int main() {
    //sleep(20);
    const IrSignal *nec1 = Nec1Renderer::newIrSignal(122, 29); // power_on for Yahama receivers
    Stream str(std::cout);
    nec1->dump(str, true);

    IrSequenceReader irSequenceReader(nec1->getIntro());
    Nec1Decoder decoder(irSequenceReader);
    decoder.printDecode(str);

    IrSenderSimulator sender(str);
    sender.sendIrSignal(*nec1, 3);

    const IrSignal *sig = Pronto::parse("0000 006C 0022 0002 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 015B 0057 0016 0E6C");
    sig->dump(str, true);

    sig = Rc5Renderer::newIrSignal(0, 1, 0);
    sig->dump(str, true);
    IrSequenceReader irSequenceReaderRc5(sig->getRepeat());
    Rc5Decoder rc5Decoder(irSequenceReaderRc5);
    rc5Decoder.printDecode(str);

}
