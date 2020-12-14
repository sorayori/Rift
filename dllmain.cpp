#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "core.h"

/*
DWORD WINAPI MainThread(LPVOID param)
{
    util::OPEN_CONSOLE();
    core::Setup();

    core::InputThread(param);

    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}
*/


void MainThread()
{
    util::OPEN_CONSOLE();
    core::Setup();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        MainThread();

        break;
    }
    return TRUE;
}