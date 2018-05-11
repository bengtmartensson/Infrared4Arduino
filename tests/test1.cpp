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
#include <sstream>

bool checkIrSignalDump(const IrSignal& irSignal, const char *ref) {
    std::ostringstream oss;
    Stream ss(oss);
    irSignal.dump(ss, true);
    return oss.str() == std::string(ref);
}

bool checkDecoderDump(const IrDecoder& decoder, const char *ref) {
    std::ostringstream oss;
    Stream ss(oss);
    decoder.printDecode(ss);
    return oss.str() == std::string(ref);
}

bool checkSenderSimulator(const IrSignal& irSignal, unsigned int noSends, const char *ref) {
    std::ostringstream oss;
    Stream ss(oss);
    IrSenderSimulator sender(ss);
    sender.sendIrSignal(irSignal, noSends);
    return oss.str() == std::string(ref);
}


int main() {
    unsigned int fails = 0;
    unsigned int successes = 0;

    // Test 1
    const IrSignal *nec1 = Nec1Renderer::newIrSignal(122, 29); // power_on for Yahama receivers
    Stream stdout(std::cout);
    nec1->dump(stdout, true);
    bool ok = checkIrSignalDump(*nec1, "f=38400\n"
            "+9024 -4512 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -39756\n"
            "+9024 -2256 +564 -65535\n\n");
    (ok ? successes : fails)++;


    // Test 2
    IrSequenceReader irSequenceReader(nec1->getIntro());
    Nec1Decoder decoder(irSequenceReader);
    decoder.printDecode(stdout);
    ok = checkDecoderDump(decoder, "NEC1 122 29\n");
    (ok ? successes : fails)++;

    // Test 3
    IrSenderSimulator sender(stdout);
    sender.sendIrSignal(*nec1, 3);
    ok = checkSenderSimulator(*nec1, 3, "IrSenderSimulator: f=38400 +9024 -4512 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -39756\n"
            "IrSenderSimulator: f=38400 +9024 -2256 +564 -65535\n"
            "IrSenderSimulator: f=38400 +9024 -2256 +564 -65535\n");
    (ok ? successes : fails)++;

    // Test 4
    const IrSignal *sig = Pronto::parse("0000 006C 0022 0002 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 015B 0057 0016 0E6C");
    sig->dump(stdout, true);
    ok = checkIrSignalDump(*sig, "f=38380\n"
            "+9040 -4507 +573 -573 +573 -1693 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -1693 +573 -573 +573 -1693 +573 -573 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -1693 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -39785\n"
            "+9040 -2266 +573 -65535\n\n");
    (ok ? successes : fails)++;

    // Test 5
    sig = Rc5Renderer::newIrSignal(0, 1, 0);
    sig->dump(stdout, true);
    ok = checkIrSignalDump(*sig, "f=36000\n\n"
            "+889 -889 +1778 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -1778 +889 -65535\n\n");
    (ok ? successes : fails)++;

    // Test 6
    IrSequenceReader irSequenceReaderRc5(sig->getRepeat());
    Rc5Decoder rc5Decoder(irSequenceReaderRc5);
    rc5Decoder.printDecode(stdout);
    ok = checkDecoderDump(rc5Decoder, "RC5 0 1 0\n");
    (ok ? successes : fails)++;

    delete nec1;

    // Test 7
    const char prontoHex[] = "0000 006C 0022 0000 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7";

    microseconds_t data[] = {
        9040, 4507, 573, 573, 573, 1693, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 1693, 573, 573, 573, 1693, 573, 573, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 1693, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 39785,
        //9040, 2266, 573, 65535
    };
    char *result = Pronto::toProntoHex(data, sizeof(data)/sizeof(microseconds_t), 38380);
    std::cout << result << std::endl;

    ok = result == std::string(prontoHex);
    (ok ? successes : fails)++;
    delete [] result;


    // Test 8
    IrSignal* irSignal = Pronto::parse(prontoHex);
    const char* s = Pronto::toProntoHex(*irSignal);
    ok = std::string(s) == std::string(prontoHex);
    (ok ? successes : fails)++;

    // Report
    std::cout << "Successes: " << successes << std::endl;
    std::cout << "Fails: " << fails << std::endl;
    exit(fails);
}
