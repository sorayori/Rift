#pragma once
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include "skCrypter.h"

#ifdef RIFT_DEVELOPMENT
#define DEBUG_LOG(input, ...) printf(input, __VA_ARGS__)
#else
#define DEBUG_LOG(input, ...)
#endif

class Util
{
public:
    static std::string upper_string(const std::string& str)
    {
        std::string upper;
        std::transform(str.begin(), str.end(), std::back_inserter(upper), toupper);
        return upper;
    }

    static __forceinline uintptr_t GET_BASE_ADDRESS()
    {
        return reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
    }

    static __forceinline void OPEN_CONSOLE()
    {
        AllocConsole();

        FILE* pFile;
        freopen_s(&pFile, skCrypt("CONOUT$"), "w", stdout);
    }
};