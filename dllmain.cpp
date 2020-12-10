#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "core.h"

DWORD WINAPI MainThread(LPVOID param)
{
    util::OPEN_CONSOLE();
    core::Setup();

    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    }
    return TRUE;
}