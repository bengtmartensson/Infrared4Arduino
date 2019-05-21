#pragma once

#include <IrSignal.h>

/**
 * A static class consisting of two functions that generate IrSignal-s from the Panasonic protocol parameters.
 * The Panasonic protocol is given in IRP notation as
 * {37k,432}<1,-1|1,-3>(8,-4,2:8,32:8,D:8,S:8,F:8,(D^S^F):8,1,-173)* [D:0..255,S:0..255,F:0..255]
 */
class PanasonicRenderer {
private:
    static const frequency_t frequency = 37000U;
    static const microseconds_t timebase = 432U;
    static const size_t dataLength = 100U;

public:

    /**
     * Generates am IrSignal from the Panasonic parameters.
     * Must be deleted explicitly by the user to avoid memory leaks.
     * @param D parameter in Panasonic, "device"
     * @param S parameter in Panasonic, "sub-device"
     * @param F parameter in Panasonic, "function"
     * @return IrSignal
     */
    static const IrSignal *newIrSignal(unsigned int D, unsigned int S, unsigned int F);

private:
    PanasonicRenderer();
    static void lsbByte(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int D);
    static void transmitBit(microseconds_t *intro, unsigned int& i, uint32_t& sum, unsigned int data);
};
