/*
    Initial author: Convery (tcn@ayria.se)
    Started: 29-04-2018
    License: MIT
    Notes:
        Fast but not actually cryptographically secure hashing.
*/

#pragma once
#include "../Stdinclude.hpp"

namespace Hash
{
    namespace Internal
    {
        constexpr uint32_t FNV1_Prime_32 = 16777619u;
        constexpr uint64_t FNV1_Prime_64 = 1099511628211u;
        constexpr uint32_t FNV1_Offset_32 = 2166136261u;
        constexpr uint64_t FNV1_Offset_64 = 14695981039346656037u;

        // Compile-time hashing for null-terminated strings.
        constexpr uint32_t FNV1_32(const char *String, const uint32_t Lastvalue = Internal::FNV1_Offset_32)
        {
            return *String ? FNV1_32(String + 1, (Lastvalue * Internal::FNV1_Prime_32) ^ *String) : Lastvalue;
        }
        constexpr uint64_t FNV1_64(const char *String, const uint64_t Lastvalue = Internal::FNV1_Offset_64)
        {
            return *String ? FNV1_64(String + 1, (Lastvalue * Internal::FNV1_Prime_64) ^ *String) : Lastvalue;
        }
        constexpr uint32_t FNV1a_32(const char *String, const uint32_t Lastvalue = Internal::FNV1_Offset_32)
        {
            return *String ? FNV1a_32(String + 1, (*String ^ Lastvalue) * Internal::FNV1_Prime_32) : Lastvalue;
        }
        constexpr uint64_t FNV1a_64(const char *String, const uint64_t Lastvalue = Internal::FNV1_Offset_64)
        {
            return *String ? FNV1a_64(String + 1, (*String ^ Lastvalue) * Internal::FNV1_Prime_64) : Lastvalue;
        }
    }

    // Compile-time hashing for fixed-length datablocks.
    constexpr uint32_t FNV1_32(const char *Input, const size_t Length)
    {
        uint32_t Hash = Internal::FNV1_Offset_32;

        for (size_t i = 0; i < Length; ++i)
        {
            Hash *= Internal::FNV1_Prime_32;
            Hash ^= Input[i];
        }

        return Hash;
    }
    constexpr uint64_t FNV1_64(const char *Input, const size_t Length)
    {
        uint64_t Hash = Internal::FNV1_Offset_64;

        for (size_t i = 0; i < Length; ++i)
        {
            Hash *= Internal::FNV1_Prime_64;
            Hash ^= Input[i];
        }

        return Hash;
    }
    constexpr uint32_t FNV1a_32(const char *Input, const size_t Length)
    {
        uint32_t Hash = Internal::FNV1_Offset_32;

        for (size_t i = 0; i < Length; ++i)
        {
            Hash ^= Input[i];
            Hash *= Internal::FNV1_Prime_32;
        }

        return Hash;
    }
    constexpr uint64_t FNV1a_64(const char *Input, const size_t Length)
    {
        uint64_t Hash = Internal::FNV1_Offset_64;

        for (size_t i = 0; i < Length; ++i)
        {
            Hash ^= Input[i];
            Hash *= Internal::FNV1_Prime_64;            
        }

        return Hash;
    }

    // Compile-time hashing for null-terminated strings.
    constexpr uint32_t FNV1_32(const char *String)
    {
        return Internal::FNV1_32(String);
    }
    constexpr uint64_t FNV1_64(const char *String)
    {
        return Internal::FNV1_64(String);
    }
    constexpr uint32_t FNV1a_32(const char *String)
    {
        return Internal::FNV1a_32(String);
    }
    constexpr uint64_t FNV1a_64(const char *String)
    {
        return Internal::FNV1a_64(String);
    }

    // Wrappers for runtime hashing of strings.
    inline uint32_t FNV1_32(const std::string &String)
    {
        return FNV1_32(String.c_str());
    }
    inline uint64_t FNV1_64(const std::string &String)
    {
        return FNV1_64(String.c_str());
    }
    inline uint32_t FNV1a_32(const std::string &String)
    {
        return FNV1a_32(String.c_str());
    }
    inline uint64_t FNV1a_64(const std::string &String)
    {
        return FNV1a_64(String.c_str());
    }
}
