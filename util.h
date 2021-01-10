#pragma once
#include <Windows.h>
#include <iostream>

#define RELATIVE_ADDR(addr, size) ((PBYTE)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))

#define ReadPointer(base, offset) (*(PVOID *)(((PBYTE)base + offset)))
#define ReadDWORD(base, offset) (*(PDWORD)(((PBYTE)base + offset)))
#define ReadBYTE(base, offset) (*(((PBYTE)base + offset)))

namespace util
{
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
}