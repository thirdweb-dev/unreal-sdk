// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Hash/Keccak256.h"

// Big endian architectures need #define __BYTE_ORDER __BIG_ENDIAN
#if !defined(_MSC_VER) && !defined(PLATFORM_MAC)
#include <endian.h>
#endif

// Constants and local helper functions
namespace
{
    constexpr unsigned int KeccakRounds = 24;

    // @formatter:off
    constexpr uint64_t XorMasks[KeccakRounds] =
    {
        0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
        0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
        0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
        0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
        0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
        0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
        0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
        0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
    };
    // @formatter:on

    // Rotate left and wrap around to the right
    inline uint64_t RotateLeft(const uint64_t x, const uint8_t numBits)
    {
        return (x << numBits) | (x >> (64 - numBits));
    }

    // Convert little vs big endian
    inline uint64_t Swap(uint64_t X)
    {
#if defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap64(X);
#endif
#ifdef _MSC_VER
        return _byteswap_uint64(X);
#endif

        // @formatter:off
        return (X >> 56) |
            ((X >> 40) & 0x000000000000FF00ULL) |
            ((X >> 24) & 0x0000000000FF0000ULL) |
            ((X >>  8) & 0x00000000FF000000ULL) |
            ((X <<  8) & 0x000000FF00000000ULL) |
            ((X << 24) & 0x0000FF0000000000ULL) |
            ((X << 40) & 0x00FF000000000000ULL) |
            (X << 56);
        // @formatter:on
    }

    /**
     * @returns x % 5 for 0 <= x <= 9
     */
    unsigned int Mod5(unsigned int X)
    {
        return X < 5 ? X : X - 5;
    }
}

FKeccak256::FKeccak256(const EBits Bits)
{
    _BlockSize = 200 - 2 * (static_cast<uint32>(Bits) / 8);
    _Bits = Bits;

    Reset();
}

void FKeccak256::Reset()
{
    for(size_t i = 0; i < StateSize; i++)
    {
        _Hash[i] = 0;
    }

    _NumBytes = 0;
    _BufferSize = 0;
}

void FKeccak256::ProcessBlock(const void* Data)
{
#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
#define LITTLEENDIAN(x) Swap(x)
#else
#define LITTLEENDIAN(x) (x)
#endif

    const uint64_t* Data64 = static_cast<const uint64_t*>(Data);

    // Mix data into state
    for(unsigned int i = 0; i < _BlockSize / 8; i++)
    {
        _Hash[i] ^= LITTLEENDIAN(Data64[i]);
    }

    // Re-compute state
    for(unsigned int Round = 0; Round < KeccakRounds; Round++)
    {
        // Theta
        uint64_t Coefficients[5];
        for(unsigned int i = 0; i < 5; i++)
        {
            Coefficients[i] = _Hash[i] ^ _Hash[i + 5] ^ _Hash[i + 10] ^ _Hash[i + 15] ^ _Hash[i + 20];
        }

        for(unsigned int i = 0; i < 5; i++)
        {
            const uint64_t One = Coefficients[Mod5(i + 4)] ^ RotateLeft(Coefficients[Mod5(i + 1)], 1);
            _Hash[i] ^= One;
            _Hash[i + 5] ^= One;
            _Hash[i + 10] ^= One;
            _Hash[i + 15] ^= One;
            _Hash[i + 20] ^= One;
        }

        // Temporary
        uint64_t One;

        // Rho Pi
        // @formatter:off
        uint64_t Last = _Hash[1];
        One = _Hash[10]; _Hash[10] = RotateLeft(Last,  1); Last = One;
        One = _Hash[ 7]; _Hash[ 7] = RotateLeft(Last,  3); Last = One;
        One = _Hash[11]; _Hash[11] = RotateLeft(Last,  6); Last = One;
        One = _Hash[17]; _Hash[17] = RotateLeft(Last, 10); Last = One;
        One = _Hash[18]; _Hash[18] = RotateLeft(Last, 15); Last = One;
        One = _Hash[ 3]; _Hash[ 3] = RotateLeft(Last, 21); Last = One;
        One = _Hash[ 5]; _Hash[ 5] = RotateLeft(Last, 28); Last = One;
        One = _Hash[16]; _Hash[16] = RotateLeft(Last, 36); Last = One;
        One = _Hash[ 8]; _Hash[ 8] = RotateLeft(Last, 45); Last = One;
        One = _Hash[21]; _Hash[21] = RotateLeft(Last, 55); Last = One;
        One = _Hash[24]; _Hash[24] = RotateLeft(Last,  2); Last = One;
        One = _Hash[ 4]; _Hash[ 4] = RotateLeft(Last, 14); Last = One;
        One = _Hash[15]; _Hash[15] = RotateLeft(Last, 27); Last = One;
        One = _Hash[23]; _Hash[23] = RotateLeft(Last, 41); Last = One;
        One = _Hash[19]; _Hash[19] = RotateLeft(Last, 56); Last = One;
        One = _Hash[13]; _Hash[13] = RotateLeft(Last,  8); Last = One;
        One = _Hash[12]; _Hash[12] = RotateLeft(Last, 25); Last = One;
        One = _Hash[ 2]; _Hash[ 2] = RotateLeft(Last, 43); Last = One;
        One = _Hash[20]; _Hash[20] = RotateLeft(Last, 62); Last = One;
        One = _Hash[14]; _Hash[14] = RotateLeft(Last, 18); Last = One;
        One = _Hash[22]; _Hash[22] = RotateLeft(Last, 39); Last = One;
        One = _Hash[ 9]; _Hash[ 9] = RotateLeft(Last, 61); Last = One;
        One = _Hash[ 6]; _Hash[ 6] = RotateLeft(Last, 20); Last = One;
                         _Hash[ 1] = RotateLeft(Last, 44);
        // @formatter:on

        // Chi
        for(unsigned int j = 0; j < StateSize; j += 5)
        {
            // Temporaries
            const uint64_t One1 = _Hash[j];
            const uint64_t Two = _Hash[j + 1];

            _Hash[j] ^= _Hash[j + 2] & ~Two;
            _Hash[j + 1] ^= _Hash[j + 3] & ~_Hash[j + 2];
            _Hash[j + 2] ^= _Hash[j + 4] & ~_Hash[j + 3];
            _Hash[j + 3] ^= One1 & ~_Hash[j + 4];
            _Hash[j + 4] ^= Two & ~One1;
        }

        // Iota
        _Hash[0] ^= XorMasks[Round];
    }
}

void FKeccak256::Add(const void* Data, size_t NumBytes)
{
    const uint8_t* Current = (uint8_t*)(Data);

    if(_BufferSize > 0)
    {
        while(NumBytes > 0 && _BufferSize < _BlockSize)
        {
            _Buffer[_BufferSize++] = *Current++;
            NumBytes--;
        }
    }

    // full buffer
    if(_BufferSize == _BlockSize)
    {
        ProcessBlock((void*)_Buffer);
        _NumBytes += _BlockSize;
        _BufferSize = 0;
    }

    // no more data ?
    if(NumBytes == 0)
    {
        return;
    }

    // process full blocks
    while(NumBytes >= _BlockSize)
    {
        ProcessBlock(Current);
        Current += _BlockSize;
        _NumBytes += _BlockSize;
        NumBytes -= _BlockSize;
    }

    // keep remaining bytes in buffer
    while(NumBytes > 0)
    {
        _Buffer[_BufferSize++] = *Current++;
        NumBytes--;
    }
}

void FKeccak256::ProcessBuffer()
{
    const unsigned int BlockSize = 200 - 2 * (static_cast<uint32>(_Bits) / 8);

    // Add padding
    size_t Offset = _BufferSize;

    // Add a "1" byte
    _Buffer[Offset++] = 1;

    // Fill with zeros
    while(Offset < BlockSize)
    {
        _Buffer[Offset++] = 0;
    }

    // And add a single set bit
    _Buffer[BlockSize - 1] |= 0x80;

    ProcessBlock(_Buffer);
}

FString FKeccak256::GetHash()
{
    // Save hash state
    uint64_t OldHash[StateSize];
    for(unsigned int i = 0; i < StateSize; i++)
    {
        OldHash[i] = _Hash[i];
    }

    // Process remaining bytes
    ProcessBuffer();

    // Convert hash to string
    static constexpr char Dec2Hex[16 + 1] = "0123456789abcdef";

    // Number of significant elements in hash (uint64_t)
    const unsigned int HashLength = static_cast<uint32>(_Bits) / 64;

    FString Result;
    for(unsigned int i = 0; i < HashLength; i++)
    {
        for(unsigned int j = 0; j < 8; j++) // 64 bits => 8 bytes
        {
            // Convert a byte to hex
            const unsigned char OneByte = static_cast<unsigned char>(_Hash[i] >> (8 * j));
            Result += Dec2Hex[OneByte >> 4];
            Result += Dec2Hex[OneByte & 15];
        }
    }

    // Keccak224's last entry in m_hash provides only 32 bits instead of 64 bits
    const unsigned int Remainder = static_cast<uint32>(_Bits) - HashLength * 64;
    unsigned int Processed = 0;
    while(Processed < Remainder)
    {
        // Convert a byte to hex
        const unsigned char OneByte = static_cast<unsigned char>(_Hash[HashLength] >> Processed);
        Result += Dec2Hex[OneByte >> 4];
        Result += Dec2Hex[OneByte & 15];

        Processed += 8;
    }

    // Restore state
    for(unsigned int i = 0; i < StateSize; i++)
    {
        _Hash[i] = OldHash[i];
    }

    return Result;
}

FString FKeccak256::KeccakFromString(const FString& Text, const bool& bIsHex)
{
    Reset();

    if(bIsHex)
    {
        TArray<uint8> Buffer;
        Buffer.Init(0, Text.Len());
        TArray<uint8> Data;
        for (auto& Ch : Text)
        {
            Data.Add(static_cast<uint8>(Ch));
        }
        Add(Data.GetData(), Data.Num());
    }
    else
    {
        const auto TextStrCast = StringCast<ANSICHAR>(*Text);
        const ANSICHAR* SingleByteArray = TextStrCast.Get();
        Add(SingleByteArray, strlen(SingleByteArray));
    }

    return GetHash();
}

FString FKeccak256::KeccakFromBytes(const TArray<uint8>& Bytes)
{
    Reset();
    Add(Bytes.GetData(), Bytes.Num());

    return GetHash();
}