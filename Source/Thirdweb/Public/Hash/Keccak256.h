// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

/**
 * This class implements the KECCAK-256 hash function.
 */
class THIRDWEB_API FKeccak256
{
public:
 
    /**
     * Algorithm variants.
     */
    enum class EBits : uint32
    {
        Keccak224 = 224,
        Keccak256 = 256,
        Keccak384 = 384,
        Keccak512 = 512
    };

private:
    /**
     * 1600 bits, stored as 25x64 bit, BlockSize is no more than 1152 bits (Keccak224).
     */
    enum
    {
        StateSize = 1600 / (8 * 8),
        MaxBlockSize = 200 - 2 * (224 / 8)
    };

    /**
     * The calculated hash.
     */
    uint64_t _Hash[StateSize];

    /**
     * Size of processed data in bytes.
     */
    uint64_t _NumBytes;

    /**
     * Block size (less or equal to MaxBlockSize).
     */
    size_t _BlockSize;

    /**
     * Valid bytes in m_buffer.
     */
    size_t _BufferSize;

    /**
     * Bytes not processed yet.
     */
    uint8_t _Buffer[MaxBlockSize];

    /**
     * Variant
     */
    EBits _Bits;

public:
    explicit FKeccak256(EBits Bits = EBits::Keccak256);

    /**
     * Generates a KECCAK-256 hash from text.
     * The result will be an Ethereum address without checksum applied.
     * 
     * @param Text The text to hash.
     * @param bIsHex If true, text is interpreted as hex digits. Otherwise, text will be interpreted as regular text.
     * @returns The generated hash.
     */
    FString KeccakFromString(const FString& Text, const bool& bIsHex);

    /**
     * Generates a KECCAK-256 hash from byte array.
     * 
     * @param Bytes The data to hash.
     * @returns The generated hash.
     */
    FString KeccakFromBytes(const TArray<uint8>& Bytes);

private:
    /**
     * Process a full block.
     *
     * @param Data The data.
     */
    void ProcessBlock(const void* Data);

    /**
     * Process everything left in the internal buffer.
     */
    void ProcessBuffer();

    /**
     * Add arbitrary number of bytes.
     * 
     * @param Data The data.
     * @param NumBytes The number of bytes in data.
     */
    void Add(const void* Data, size_t NumBytes);

    /**
     * @returns The latest hash as 16 hex characters.
     */
    FString GetHash();

    /**
     * Resets buffer and calculated hash.
     */
    void Reset();
};