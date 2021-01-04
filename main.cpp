#define _CRT_SECURE_NO_WARNINGS

#include "core.h"

void MainThread()
{
#ifdef RIFT_DEVELOPMENT
    Util::OPEN_CONSOLE();
#endif
    if (!Core::Init())
        DEBUG_LOG("Init failed, one or more addresses are most likely incorrect!\n");
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