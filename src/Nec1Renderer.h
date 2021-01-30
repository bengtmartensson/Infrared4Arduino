#pragma once

#include <IrSignal.h>

/**
 * A static class consisting of two functions that generate IrSignal-s from the NEC1 protocol parameters.
 * The NEC1 protocol is given in IRP notation as
 * \c {38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,1,^108m,(16,-4,1,^108m)*) [D:0..255,S:0..255=255-D,F:0..255]
 */
class Nec1Renderer {
private:
    static constexpr frequency_t frequency = 38400UL;
    static constexpr dutycycle_t dutyCycle = 33U;
    static constexpr size_t introLength = 68U;
    static constexpr size_t repeatLength = 4U;

public:

    /**
     * Generates am IrSignal from the NEC1 parameters.
     * Must be deleted explicitly by the user to avoid memory leaks.
     * @param D parameter in NEC1, "device"
     * @param S parameter in NEC1, "sub-device"
     * @param F parameter in NEC1, "function"
     * @return IrSignal
     */
    static const IrSignal *newIrSignal(unsigned int D, unsigned int S, unsigned int F);

    /**
     * Generates am IrSignal from the NEC1 parameters. Equivalent to newIrSignal(D, 255-D, F).
     * Must be deleted explicitly by the user to avoid memory leaks.
     * @param D parameter in NEC1, "device"
     * @param F parameter in NEC1, "function"
     * @return IrSignal
     */
    static const IrSignal *newIrSignal(unsigned int D, unsigned int F) {
        return newIrSignal(D, 255-D, F);
    };

private:
    Nec1Renderer();
    static void lsbByte(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int D);
    static void transmitBit(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int data);
};
