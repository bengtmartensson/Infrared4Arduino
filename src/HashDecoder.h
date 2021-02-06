/**
 * @file HashDecoder.h
 * @brief This file defines a hash based "decoder class".
 */

#pragma once

#include "IrDecoder.h"
#include "IrReader.h"
#include "IrSignal.h"

/**
 * A decoder class using <a href="https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function">FNV-1</a>
 * hashes of length 32.
 * This is not a decoder in the proper sense of the word,
 * but instead computes a hash value from the IrSequence.
 * For different IrSequences, this will, with high probability, be different.
 */
class HashDecoder : public IrDecoder {
private:
    uint32_t hash;

    char decodeBuffer[2 * sizeof (hash) + 1];

    static uint32_t compare(microseconds_t oldval, microseconds_t newval);

    void decode(const microseconds_t* data, size_t length);

    void decode(const IrReader& irReader);

    void decode(const IrSequence& irSequence) {
        decode(irSequence.getDurations(), irSequence.getLength());
    }

    void decode(const IrSignal& irSignal) {
        decode(irSignal.getIntro());
        decode(irSignal.getRepeat());
        decode(irSignal.getEnding());
    }

    static constexpr unsigned int offset = 2;
    static constexpr const char *format = "%0lx";
    static constexpr uint32_t FNVprime = 16777619UL;
    static constexpr uint32_t FNVoffsetBasis = 2166136261UL;
    static constexpr unsigned int minMeaningfulLength = 4U;

public:

#ifndef DOXYGEN
    HashDecoder() = delete;
    HashDecoder(const HashDecoder&) = delete;
    HashDecoder(HashDecoder&&) = delete;
    HashDecoder& operator=(const HashDecoder&) = delete;
    HashDecoder& operator=(HashDecoder&&) = delete;
#endif // ! DOXYGEN

    virtual ~HashDecoder() {}

    /**
     * Constructs a HashDecoder from an IrReader, containing data.
     * @param irReader IrReader with data, i.e. with isReady() true.
     */
    HashDecoder(const IrReader& irReader) : IrDecoder(),hash(FNVoffsetBasis) {
        decode(irReader);
        sprintf(decodeBuffer, format, static_cast<unsigned long>(hash));
    }

    /**
     * Constructs a HashDecoder from an IrSequence.
     * @param irSequence
     */
    HashDecoder(const IrSequence& irSequence) : IrDecoder(),hash(FNVoffsetBasis) {
        decode(irSequence);
        sprintf(decodeBuffer, format, static_cast<unsigned long>(hash));
    }

    HashDecoder(const IrSignal& irSignal) : IrDecoder(),hash(FNVoffsetBasis) {
        decode(irSignal);
        sprintf(decodeBuffer, format, static_cast<unsigned long>(hash));
    }

    /**
     * Returns the hash value.
     * @return uint32_t
     */
    uint32_t getHash() const {
        return hash;
    }

    static uint32_t decodeHash(const IrSequence& irSequence);

    static uint32_t decodeHash(const IrReader& irReader);

    /**
     * Convenience function; constructs a HashDecoder and calls its printDecode.
     * @param irReader IrReader to use
     * @param stream Stream
     * @return success of operation
     */
    static bool tryDecode(const IrReader& irReader, Stream& stream);

    const char *getDecode() const {
        return decodeBuffer;
    }
};
