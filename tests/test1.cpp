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

bool checkDecoderDump(bool verbose, const IrDecoder& decoder, const char *ref) {
    if (verbose) {
        Stream stdout(std::cout);
        decoder.printDecode(stdout);
    }
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

static bool testSignalRenderer(bool verbose, const IrSignal *signal, const char* str) {
    if (verbose) {
        Stream stdout(std::cout);
        signal->dump(stdout, true);
    }
    return checkIrSignalDump(*signal, str);
}

static bool testNec1Renderer(bool verbose) {
    const IrSignal *nec1 = Nec1Renderer::newIrSignal(122, 29); // power_on for Yahama receivers
    return testSignalRenderer(verbose, nec1, "f=38400\n"
            "+9024 -4512 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -39756\n"
            "+9024 -2256 +564 -65535\n\n");
}

static bool testRc5Renderer(bool verbose) {
    const IrSignal *sig = Rc5Renderer::newIrSignal(0, 1, 0);
    return testSignalRenderer(verbose, sig, "f=36000\n\n"
            "+889 -889 +1778 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -889 +889 -1778 +889 -65535\n\n");
}

static bool testNec1Decoder(bool verbose) {
    const IrSignal *nec1 = Nec1Renderer::newIrSignal(122, 29); // power_on for Yahama receivers
    IrSequenceReader irSequenceReader(nec1->getIntro());
    Nec1Decoder decoder(irSequenceReader);
    return checkDecoderDump(verbose, decoder, "NEC1 122 29\n");
}

static bool testRc5Decoder(bool verbose) {
    const IrSignal *sig = Rc5Renderer::newIrSignal(0, 1, 0);
    IrSequenceReader irSequenceReaderRc5(sig->getRepeat());
    Rc5Decoder rc5Decoder(irSequenceReaderRc5);
    return checkDecoderDump(verbose, rc5Decoder, "RC5 0 1 0\n");
}

static bool testIrSenderSimulator(bool verbose) {
    const IrSignal *nec1 = Nec1Renderer::newIrSignal(122, 29); // power_on for Yahama receivers
    if (verbose) {
        Stream stdout(std::cout);
        IrSenderSimulator sender(stdout);
        sender.sendIrSignal(*nec1, 3);
    }
    return checkSenderSimulator(*nec1, 3, "IrSenderSimulator: f=38400 +9024 -4512 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -564 +564 -564 +564 -564 +564 -1692 +564 -1692 +564 -1692 +564 -39756\n"
            "IrSenderSimulator: f=38400 +9024 -2256 +564 -65535\n"
            "IrSenderSimulator: f=38400 +9024 -2256 +564 -65535\n");
}

static bool testPronto(bool verbose) {
    const IrSignal *sig = Pronto::parse("0000 006C 0022 0002 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 015B 0057 0016 0E6C");
    if (verbose) {
        Stream stdout(std::cout);
        sig->dump(stdout, true);
    }
    return checkIrSignalDump(*sig, "f=38380\n"
            "+9040 -4507 +573 -573 +573 -1693 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -1693 +573 -573 +573 -1693 +573 -573 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -1693 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -573 +573 -573 +573 -573 +573 -1693 +573 -1693 +573 -1693 +573 -39785\n"
            "+9040 -2266 +573 -65535\n\n");
}

static bool testToProntoHex(bool verbose) {
    const char prontoHex[] = "0000 006C 0022 0000 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7";
    microseconds_t data[] =
    {
        9040, 4507, 573, 573, 573, 1693, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 1693, 573, 573, 573, 1693, 573, 573, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 1693, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 573, 573, 573, 573, 573, 573, 1693, 573, 1693, 573, 1693, 573, 39785,
        //9040, 2266, 573, 65535
    };
    char *result = Pronto::toProntoHex(data, sizeof (data) / sizeof (microseconds_t), 38380);
    if (verbose)
        std::cout << result << std::endl;

    bool ok = result == std::string(prontoHex);
    delete [] result;
    return ok;
}

static bool testProntoParse(bool verbose __attribute__((unused))) {
    const char prontoHex[] = "0000 006C 0022 0000 015B 00AD 0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7";
    IrSignal* irSignal = Pronto::parse(prontoHex);
    const char* s = Pronto::toProntoHex(*irSignal);
    return std::string(s) == std::string(prontoHex);
}

#define TEST(f) if (f(verbose)) {successes++;} else {std::cout << #f << " failed!" << std::endl; fails++;}

int main(int argc, const char *args[] __attribute__((unused))) {
    bool verbose = argc > 1;
    unsigned int fails = 0;
    unsigned int successes = 0;

    TEST(testNec1Renderer);
    TEST(testRc5Renderer);   
    TEST(testNec1Decoder);
    TEST(testRc5Decoder);                  
    TEST(testIrSenderSimulator);     
    TEST(testPronto);                
    TEST(testToProntoHex);
    TEST(testProntoParse);           

    // Report
    std::cout << "Successes: " << successes << std::endl;
    std::cout << "Fails: " << fails << std::endl;
    exit(fails);
}
