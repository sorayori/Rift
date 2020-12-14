#pragma once
#include <iostream>

enum class ESpawnActorCollisionHandlingMethod : uint8_t
{
	Undefined = 0,
	AlwaysSpawn = 1,
	AdjustIfPossibleButAlwaysSpawn = 2,
	AdjustIfPossibleButDontSpawnIfColliding = 3,
	DontSpawnIfColliding = 4,
	ESpawnActorCollisionHandlingMethod_MAX = 5
};

enum class EFortQuickBars : uint8_t
{
	Primary = 0,
	Secondary = 1,
	Creative = 2,
	Max_None = 3,
	EFortQuickBars_MAX = 4
};