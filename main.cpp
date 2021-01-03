#define _CRT_SECURE_NO_WARNINGS

#include "core.h"

void MainThread()
{
    Util::OPEN_CONSOLE();
    Core::Init();
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