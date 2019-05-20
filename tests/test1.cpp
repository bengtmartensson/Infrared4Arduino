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

static bool testNec1Renderer() {
    const IrSignal *nec1 = Nec1Renderer::newIrSignal(122, 29); // power_on for Yahama receivers
    Stream stdout(std::cout);
    nec1->dump(stdout, true);
    return checkIrSignalDump(*nec1, "f=38400\n"
            "+9024 -4512 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -39756\n"
            "+9024 -2256 +564 -65535\n\n");
}

static bool testNec1Decoder() {
    const IrSignal *nec1 = Nec1Renderer::newIrSignal(122, 29); // power_on for Yahama receivers
    IrSequenceReader irSequenceReader(nec1->getIntro());
    Nec1Decoder decoder(irSequenceReader);
    Stream stdout(std::cout);
    decoder.printDecode(stdout);
    return checkDecoderDump(decoder, "NEC1 122 29\n");
}

static bool testIrSenderSimulator() {
    Stream stdout(std::cout);
    IrSenderSimulator sender(stdout);
    const IrSignal *nec1 = Nec1Renderer::newIrSignal(122, 29); // power_on for Yahama receivers
    sender.sendIrSignal(*nec1, 3);
    return checkSenderSimulator(*nec1, 3, "IrSenderSimulator: f=38400 +9024 -4512 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -39756\n"
            "IrSenderSimulator: f=38400 +9024 -2256 +564 -65535\n"
            "IrSenderSimulator: f=38400 +9024 -2256 +564 -65535\n");
}

static bool testPronto() {
    const IrSignal *sig = Pronto::parse("0000 006C 0022 0002 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 015B 0057 0016 0E6C");
    Stream stdout(std::cout);
    sig->dump(stdout, true);
    return checkIrSignalDump(*sig, "f=38380\n"
            "+9040 -4507 +573 -573 +573 -1693 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -1693 +573 -573 +573 -1693 +573 -573 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -1693 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -39785\n"
            "+9040 -2266 +573 -65535\n\n");
}

static bool testRc5Renderer() {
    const IrSignal *sig = Rc5Renderer::newIrSignal(0, 1, 0);
    Stream stdout(std::cout);
    sig->dump(stdout, true);
    return checkIrSignalDump(*sig, "f=36000\n\n"
            "+889 -889 +1778 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -1778 +889 -65535\n\n");
}

static bool testRc5Decoder() {
    const IrSignal *sig = Rc5Renderer::newIrSignal(0, 1, 0);
    IrSequenceReader irSequenceReaderRc5(sig->getRepeat());
    Rc5Decoder rc5Decoder(irSequenceReaderRc5);
    Stream stdout(std::cout);
    rc5Decoder.printDecode(stdout);
    return checkDecoderDump(rc5Decoder, "RC5 0 1 0\n");
}

static bool testToProntoHex() {
    const char prontoHex[] = "0000 006C 0022 0000 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7";
    microseconds_t data[] =
    {
        9040, 4507, 573, 573, 573, 1693, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 1693, 573, 573, 573, 1693, 573, 573, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 1693, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 39785,
        //9040, 2266, 573, 65535
    };
    char *result = Pronto::toProntoHex(data, sizeof (data) / sizeof (microseconds_t), 38380);
    std::cout << result << std::endl;

    bool ok = result == std::string(prontoHex);
    delete [] result;
    return ok;
}

static bool testProntoParse() {
    const char prontoHex[] = "0000 006C 0022 0000 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7";
    IrSignal* irSignal = Pronto::parse(prontoHex);
    const char* s = Pronto::toProntoHex(*irSignal);
    return std::string(s) == std::string(prontoHex);
}

int main() {
    unsigned int fails = 0;
    unsigned int successes = 0;

    (testNec1Renderer()         ? successes : fails)++;
    (testNec1Decoder()          ? successes : fails)++;
    (testIrSenderSimulator()    ? successes : fails)++;
    (testPronto()               ? successes : fails)++;
    (testRc5Renderer()          ? successes : fails)++;
    (testRc5Decoder()           ? successes : fails)++;
    (testToProntoHex()          ? successes : fails)++;
    (testProntoParse()          ? successes : fails)++;

    // Report
    std::cout << "Successes: " << successes << std::endl;
    std::cout << "Fails: " << fails << std::endl;
    exit(fails);
}
