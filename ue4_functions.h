#pragma once
#include <Windows.h>
#include <iostream>
#include "offsets.h"
#include "util.h"

namespace UE4
{
	static void* GetFirstPlayerController(void* World)
	{
        auto fGetFirstPlayerController = reinterpret_cast<void* (__fastcall*)(void*)>(util::GET_BASE_ADDRESS() + GETFIRSTPLAYERCONTROLLER_OFFSET);
        return fGetFirstPlayerController(World);
	}

	static void* StaticConstructObject_Internal(
        void* Class,
        void* InOuter,
        void* Name,
        int SetFlags,
        unsigned int InternalSetFlags,
        void* Template,
        bool bCopyTransientsFromClassDefaults,
        void* InstanceGraph,
        bool bAssumeTemplateIsArchetype)
	{
        auto fStaticConstructObject_Internal = reinterpret_cast<void* (__fastcall*)(void*, void*, void*, int, unsigned int, void*, bool, void*, bool)>(util::GET_BASE_ADDRESS() + SCOI_OFFSET);
        return fStaticConstructObject_Internal(Class, InOuter, Name, SetFlags, InternalSetFlags, Template, bCopyTransientsFromClassDefaults, InstanceGraph, bAssumeTemplateIsArchetype);
	}

    static void* ProcessEvent(void* obj, void* func, void* param, void* ret)
    {
        auto fProcessEvent = reinterpret_cast<void* (__fastcall*)(void*, void*, void*, void*)>(util::GET_BASE_ADDRESS() + PROCESSEVENT_OFFSET);
        return fProcessEvent(obj, func, param, ret);
    }
}