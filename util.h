#pragma once
#include <Windows.h>
#include <iostream>

class Util
{
public:
    static __forceinline uintptr_t GET_BASE_ADDRESS()
    {
        return reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
    }

    static __forceinline void OPEN_CONSOLE()
    {
        AllocConsole();

        FILE* pFile;
        freopen_s(&pFile, "CONOUT$", "w", stdout);
    }
};