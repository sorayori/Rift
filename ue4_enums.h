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

enum EMovementMode
{
	MOVE_None,
	MOVE_Walking,
	MOVE_NavWalking,
	MOVE_Falling,
	MOVE_Swimming,
	MOVE_Flying,
	MOVE_Custom,
	MOVE_MAX,
};

enum class EFortMovementStyle : uint8_t
{
	Running = 0,
	Walking = 1,
	Charging = 2,
	Sprinting = 3,
	PersonalVehicle = 4,
	Flying = 5,
	Tethered = 6,
	EFortMovementStyle_MAX = 7
};