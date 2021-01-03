#pragma once

#include "memory.h"

// Before messing with this, please remember to follow the EXACT format, any mistakes will break everything.

class FNPatterns
{
public:

	// FUObjectArray
	static constexpr const char* UObjectArrayPattern = "49 63 C8 48 8D 14 40 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1"; // (+ 0xA)

	// UKismetSystemLibrary::GetObjectName
	static constexpr const char* GetObjectNamePattern = "40 53 48 83 EC 20 48 8B D9 48 85 D2 75 45 33 C0 48 89 01 48 89 41 08 8D 50 05 E8 ? ? ? ? 8B 53 08 8D 42 05 89 43 08 3B 43 0C 7E 08 48 8B CB E8 ? ? ? ? 48 8B 0B 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B C3 48 83 C4 20 5B C3 48 8B 42 18";

	// FName::ToString
	static constexpr const char* GetNameByIndexPattern = "48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 30 8B"; // Not really using this as of now, but here just incase we want to implement something FortUpdater-style.

	// FMemory::Free	
	static constexpr const char* MemoryFreePattern = "48 85 C9 74 2E 53";

	// UWorld::GetFirstPlayerController
	static constexpr const char* GetFirstPlayerControllerPattern = "83 B9 ? ? ? ? ? 7E ? 48 8B 89 ? ? ? ? E9";

	// UWorld* GWorld
	static constexpr const char* GWorldPattern = "48 89 05 ? ? ? ? 48 8B B6 ? ? ? ?"; // (+ 0x3)

	// UObject::ProcessEvent
	static constexpr const char* ProcessEventPattern = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A";

	// StaticConstructObject_Internal
	static constexpr const char* StaticConstructObject_InternalPattern = "4C 89 44 24 18 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4";

	// UWorld::SpawnActor
	static constexpr const char* SpawnActorPattern = "53 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 60 0F 28 1D ? ? ? ? 0F 57 D2 48 8B B4 24 B0";

	// StaticLoadObject
	static constexpr const char* StaticLoadObjectPattern = "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 8B 85 ? ? ? ? 48 8B FA";

	// mov [rcx + Offsets::CurrentMovementStyleOffset], dl
	// test rbx, rbx
	// je
	static constexpr const char* SprintPatchPattern = "88 91 ? ? ? ? 48 85 DB 0F 84"; // replace with 90 90 90 90 90 90 48 85 DB 0F 84

	// Weapon patch, required.
	static constexpr const char* WeaponPatchPattern = "0F 84 ? ? ? ? 0F 2F F7 0F 86 ? ? ? ? 48 8B 00"; // replace with 0F 85 ? ? ? ? 0F 2F F7 0F 8D ? ? ? ? 48 8B 00
};