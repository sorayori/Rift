#pragma once
#include "memory.h"
#include "offsets.h"
#include "ue4.h"
#include "detours.h"
#include "skCrypter.h"
#include "api.h"

namespace Core
{
	UE4::GObjects* GlobalObjects;
	UE4::UObject** GWorld;

	UE4::UObject* CheatManager;
	UE4::UObject* Controller;
	UE4::UObject* Pawn;
	UE4::UObject* GameState;
	UE4::UObject* GameMode;
	UE4::UObject* PlayerState;
	UE4::UObject* CharacterMovement;

	UE4::UObject* CurrentEmote;
	UE4::FVector CurrentEmotePositon;

	namespace Offsets
	{
		DWORD CurrentMovementStyleOffset;
		DWORD CharacterMovementOffset;
		DWORD CheatManagerOffset;
		DWORD bInfiniteAmmoOffset;
		DWORD PawnPlayerStateOffset;
		DWORD CurrentPlaylistInfoOffset;
		DWORD BasePlaylistOffset;
		DWORD bWantsToSprintOffset;
		DWORD bSkipWarmupOffset;
		DWORD bSkipAircraftOffset;
		DWORD McpProfileGroupOffset;
		DWORD PlayerNameOffset;
		DWORD CurrentWeaponOffset;
		DWORD bIsTargetingOffset;
		DWORD WeaponDefinitionOffset;
		DWORD CharacterMeshOffset;
		DWORD CharacterGenderOffset;
		DWORD CharacterBodyTypeOffset;
		DWORD MaxNumItemsInCreativeChestsOffset;
		DWORD DefaultFlagRegionIdOffset;

		UE4::UObject* CheatManagerClass;

		UE4::UObject* Default__FortKismetLibrary;
		UE4::UObject* Default__FortItemAndVariantSwapHelpers;
		UE4::UObject* Default__FortRuntimeOptions;

		UE4::UObject* UnCrouchFunc;
		UE4::UObject* CrouchFunc;
		UE4::UObject* GetRuntimeOptionsFunc;
		UE4::UObject* PlayerPawnAthenaTickFunc;
		UE4::UObject* IsFallingFunc;
		UE4::UObject* GetCurrentMontageFunc;
		UE4::UObject* GetAnimInstanceFunc;
		UE4::UObject* Montage_StopFunc;
		UE4::UObject* PlayLocalAnimMontageFunc;
		UE4::UObject* GetAnimationHardReferenceFunc;
		UE4::UObject* PlayEmoteItemFunc;
		UE4::UObject* ServerPlayEmoteItemFunc;
		UE4::UObject* SetTimeOfDayManagerGameplayOverrideFunc;
		UE4::UObject* BugItGoFunc;
		UE4::UObject* GetActorLocationFunc;
		UE4::UObject* JumpFunc;
		UE4::UObject* JumpToSafeZonePhaseFunc;
		UE4::UObject* OnRep_CurrentPlaylistInfoFunc;
		UE4::UObject* TeleportToSkyDiveFunc;
		UE4::UObject* EquipWeaponDefinitionFunc;
		UE4::UObject* PushCosmeticOverrideOntoPawnFunc;
		UE4::UObject* SetCanBeDamagedFunc;
		UE4::UObject* DestroyAllFunc;
		UE4::UObject* StartMatchFunc;
		UE4::UObject* ServerReadyToStartMatchFunc;
		UE4::UObject* PossessFunc;
		UE4::UObject* PlayButtonFunc;
		UE4::UObject* ReadyToStartMatchFunc;
		UE4::UObject* SummonFunc;
		UE4::UObject* SwitchLevelFunc;
		UE4::UObject* IsSkydivingFunc;
		UE4::UObject* IsParachuteOpenFunc;
		UE4::UObject* IsParachuteForcedOpenFunc;
		UE4::UObject* OnRep_IsParachuteOpenFunc;
		UE4::UObject* SetMovementModeFunc;
	}

	struct RBitField
	{
		unsigned char A : 1;
		unsigned char B : 1;
		unsigned char C : 1;
		unsigned char D : 1;
		unsigned char E : 1;
		unsigned char F : 1;
		unsigned char G : 1;
		unsigned char H : 1;
	};

	class RiftAutomationUtils
	{
	public:
		static void Summon(UE4::UObject* InCheatManager, const char* ClassToSummon) 
		{
			std::string StrClassToSummon(ClassToSummon);
			UE4::FString FStrClassToSummon = UE4::FString(std::wstring(StrClassToSummon.begin(), StrClassToSummon.end()).c_str());
			UE4::ProcessEvent(InCheatManager, Offsets::SummonFunc, &FStrClassToSummon, 0);
		}

		static void Possess(UE4::UObject* InController, UE4::UObject* InPawn) { UE4::ProcessEvent(InController, Offsets::PossessFunc, &InPawn, 0); }

		static void ServerReadyToStartMatch(UE4::UObject* InController) { UE4::ProcessEvent(InController, Offsets::ServerReadyToStartMatchFunc, nullptr, 0); }

		static void StartMatch(UE4::UObject* InGameMode) { UE4::ProcessEvent(InGameMode, Offsets::StartMatchFunc, nullptr, 0); }

		static void DestroyAll(UE4::UObject* InCheatManager, const char* ClassToDestroy)
		{
			UE4::UObject* LocatedClass = GlobalObjects->FindObject(ClassToDestroy);
			UE4::ProcessEvent(InCheatManager, Offsets::DestroyAllFunc, &LocatedClass, 0);
		}

		static void SetActorGodMode(UE4::UObject* InActor, bool bGodMode)
		{
			struct
			{
				UE4::UObject* Actor;
				bool bCanBeDamaged;
			} SetCanBeDamagedParams;

			SetCanBeDamagedParams.Actor = InActor;
			SetCanBeDamagedParams.bCanBeDamaged = !bGodMode;
			UE4::ProcessEvent(Offsets::Default__FortKismetLibrary, Offsets::SetCanBeDamagedFunc, &SetCanBeDamagedParams, 0);
		}

		static void OverridePawnCosmetic(UE4::UObject* InFortPlayerPawn, const char* CosmeticFullName, bool bCaseInsensitive = false)
		{
			UE4::UObject* Cosmetic;

			if (bCaseInsensitive)
				Cosmetic = GlobalObjects->FindObjectByFullNameInsensitive(CosmeticFullName);
			else
				Cosmetic = GlobalObjects->FindObjectByFullName(CosmeticFullName);

			if (!Cosmetic)
			{
				DEBUG_LOG("RIFT: Invalid Cosmetic Name!\n");
				return;
			}

			struct
			{
				UE4::UObject* Pawn;
				UE4::UObject* Cosmetic;
			} PushCosmeticOverrideOntoPawnParams;

			PushCosmeticOverrideOntoPawnParams.Pawn = InFortPlayerPawn;
			PushCosmeticOverrideOntoPawnParams.Cosmetic = Cosmetic;

			UE4::ProcessEvent(Offsets::Default__FortItemAndVariantSwapHelpers, Offsets::PushCosmeticOverrideOntoPawnFunc, &PushCosmeticOverrideOntoPawnParams, 0);
		}

		// Return value is broke on EquipWeaponDefinition, it's insanely old and as far as I'm aware it was never actually used in-game so I'm not surprised if it's deprecated.
		// Sometime we should definitely look for an alternative, so called alternative could be our solution to fixed weapon sounds.
		static void EquipWeapon(UE4::UObject* InPawn, const char* WIDFullName)
		{
			auto WeaponToEquip = GlobalObjects->FindObjectByFullName(WIDFullName);
			if (!WeaponToEquip)
			{
				DEBUG_LOG("RIFT: Invalid weapon definition!\n");
				return;
			}

			struct
			{
				UE4::UObject* WeaponData;
				UE4::FGuid ItemEntryGuid;
				UE4::UObject* ReturnValue;
			} EquipWeaponDefinitionParams;
			EquipWeaponDefinitionParams.WeaponData = WeaponToEquip;
			UE4::FGuid NewGUID;
			NewGUID.A = rand() % 1000;
			NewGUID.B = rand() % 1000;
			NewGUID.C = rand() % 1000;
			NewGUID.D = rand() % 1000;
			EquipWeaponDefinitionParams.ItemEntryGuid = NewGUID;

			UE4::ProcessEvent(InPawn, Offsets::EquipWeaponDefinitionFunc, &EquipWeaponDefinitionParams, 0);
		}

		static void EquipWeaponFromDefinition(UE4::UObject* InPawn, UE4::UObject* WeaponDef)
		{
			if (!WeaponDef)
			{
				DEBUG_LOG("RIFT: Invalid weapon definition!\n");
				return;
			}

			struct
			{
				UE4::UObject* WeaponData;
				UE4::FGuid ItemEntryGuid;
				UE4::UObject* ReturnValue;
			} EquipWeaponDefinitionParams;
			EquipWeaponDefinitionParams.WeaponData = WeaponDef;
			UE4::FGuid NewGUID;
			NewGUID.A = rand() % 1000;
			NewGUID.B = rand() % 1000;
			NewGUID.C = rand() % 1000;
			NewGUID.D = rand() % 1000;
			EquipWeaponDefinitionParams.ItemEntryGuid = NewGUID;

			UE4::ProcessEvent(InPawn, Offsets::EquipWeaponDefinitionFunc, &EquipWeaponDefinitionParams, 0);
		}

		static void TeleportToSkyDive(UE4::UObject* InPawn, float InHeight) { UE4::ProcessEvent(InPawn, Offsets::TeleportToSkyDiveFunc, &InHeight, 0); }

		static UE4::UObject* GetPlayerState(UE4::UObject* InController)
		{
			UE4::UObject** PlayerState = reinterpret_cast<UE4::UObject**>(__int64(InController) + __int64(Offsets::PawnPlayerStateOffset));
			return *PlayerState;
		}

		static UE4::UObject* GetCharacterMovementComponent(UE4::UObject* InPawn)
		{
			return *reinterpret_cast<UE4::UObject**>(__int64(InPawn) + __int64(Offsets::CharacterMovementOffset));
		}

		static void SetMovementMode(UE4::UObject* InCharacterMovementComponent, uint8_t InNewMovementMode, unsigned char InNewCustomMode)
		{
			struct
			{
				uint8_t NewMovementMode;
				unsigned char NewCustomMode;
			} SetMovementModeParams;
			SetMovementModeParams.NewMovementMode = InNewMovementMode;
			SetMovementModeParams.NewCustomMode = InNewCustomMode;
			UE4::ProcessEvent(InCharacterMovementComponent, Offsets::SetMovementModeFunc, &SetMovementModeParams, 0);
		}

		static void SetCurrentMovementStyle(UE4::UObject* InPawn, uint8_t MovementStyle)
		{
			uint8_t* CurrentMovementStyle = reinterpret_cast<uint8_t*>(__int64(InPawn) + __int64(Offsets::CurrentMovementStyleOffset));
			*CurrentMovementStyle = MovementStyle;
		}

		static void JumpToSafeZonePhase(UE4::UObject* InortGameModeAthena) { UE4::ProcessEvent(InortGameModeAthena, Offsets::JumpToSafeZonePhaseFunc, nullptr, 0); }

		static void SetPlaylist(UE4::UObject* InFortGameStateAthena, const char* FortPlaylistAthenaFullName, bool ShouldSkipWarmup, bool ShouldSkipAircraft) 
		{
			auto FortPlaylistAthena = GlobalObjects->FindObjectByFullName(FortPlaylistAthenaFullName);

			if (!FortPlaylistAthena)
			{
				DEBUG_LOG("RIFT: Invalid playlist name!\n");
				return;
			}

			bool* bSkipWarmup = reinterpret_cast<bool*>(__int64(FortPlaylistAthena) + __int64(Offsets::bSkipWarmupOffset));
			*bSkipWarmup = ShouldSkipWarmup;

			bool* bSkipAircraft = reinterpret_cast<bool*>(__int64(FortPlaylistAthena) + __int64(Offsets::bSkipAircraftOffset));
			*bSkipAircraft = ShouldSkipAircraft;


			UE4::UObject** BasePlaylist = reinterpret_cast<UE4::UObject**>(__int64(InFortGameStateAthena) + __int64(Offsets::CurrentPlaylistInfoOffset) + __int64(Offsets::BasePlaylistOffset));
			*BasePlaylist = FortPlaylistAthena;
			UE4::ProcessEvent(InFortGameStateAthena, Offsets::OnRep_CurrentPlaylistInfoFunc, nullptr, 0);
		}

		static void SetInfiniteAmmo(UE4::UObject* InController)
		{
			RBitField* BitField = reinterpret_cast<RBitField*>(__int64(InController) + __int64(Offsets::bInfiniteAmmoOffset));
			BitField->C = 1;
		}

		static UE4::UObject* CreateCheatManager(UE4::UObject* InController)
		{
			UE4::UObject* NewCheatManager = UE4::StaticConstructObject_Internal(Offsets::CheatManagerClass, InController, 0, 0, 0, 0, 0, 0, 0);
			UE4::UObject** CheatManager = reinterpret_cast<UE4::UObject**>(__int64(InController) + __int64(Offsets::CheatManagerOffset));
			*CheatManager = NewCheatManager;
			return NewCheatManager;
		}

		static void SwitchLevel(UE4::UObject* InController, const char* InURL)
		{
			std::string StrInURL(InURL);
			UE4::FString FStrInURL = UE4::FString(std::wstring(StrInURL.begin(), StrInURL.end()).c_str());
			UE4::ProcessEvent(InController, Offsets::SwitchLevelFunc, &FStrInURL, 0);
		}

		static void Jump(UE4::UObject* InPawn) { UE4::ProcessEvent(InPawn, Offsets::JumpFunc, nullptr, 0); }

		static bool IsSkydiving(UE4::UObject* InPawn)
		{
			bool ReturnValue;
			UE4::ProcessEvent(InPawn, Offsets::IsSkydivingFunc, &ReturnValue, 0);
			return ReturnValue;
		}
		static bool IsParachuteOpen(UE4::UObject* InPawn)
		{
			bool ReturnValue;
			UE4::ProcessEvent(InPawn, Offsets::IsParachuteOpenFunc, &ReturnValue, 0);
			return ReturnValue;
		}
		static bool IsParachuteForcedOpen(UE4::UObject* InPawn)
		{
			bool ReturnValue;
			UE4::ProcessEvent(InPawn, Offsets::IsParachuteForcedOpenFunc, &ReturnValue, 0);
			return ReturnValue;
		}

		static void OnRep_IsParachuteOpen(UE4::UObject* InPawn, bool bPreviousState)
		{
			UE4::ProcessEvent(InPawn, Offsets::OnRep_IsParachuteOpenFunc, &bPreviousState, 0);
		}

		static UE4::FVector GetActorLocation(UE4::UObject* InActor)
		{
			UE4::FVector ReturnValue;
			UE4::ProcessEvent(InActor, Offsets::GetActorLocationFunc, &ReturnValue, 0);
			return ReturnValue;
		}

		static void BugItGo(UE4::UObject* InCheatManager, float InX, float InY, float InZ, float InPitch, float InYaw, float InRoll)
		{
			struct
			{
				float X;
				float Y;
				float Z;
				float Pitch;
				float Yaw;
				float Roll;
			} BugItGoParams;
			BugItGoParams.X = InX;
			BugItGoParams.Y = InY;
			BugItGoParams.Z = InZ;
			BugItGoParams.Pitch = InPitch;
			BugItGoParams.Yaw = InYaw;
			BugItGoParams.Roll = InRoll;
			UE4::ProcessEvent(InCheatManager, Offsets::BugItGoFunc, &BugItGoParams, 0);
		}

		static bool WantsToSprint(UE4::UObject* InController)
		{
			RBitField* BitField = reinterpret_cast<RBitField*>(__int64(InController) + __int64(Offsets::bWantsToSprintOffset));
			return BitField->A;
		}

		static std::string GetAccountName()
		{
			UE4::UObject* Controller = (UE4::UObject*)(UE4::GetFirstPlayerController(*GWorld));
			UE4::UObject* McpProfileGroup = *reinterpret_cast<UE4::UObject**>(__int64(Controller) + __int64(Offsets::McpProfileGroupOffset));
			UE4::FString PlayerName = *reinterpret_cast<UE4::FString*>(__int64(McpProfileGroup) + __int64(Offsets::PlayerNameOffset));
			return PlayerName.ToString();
		}

		static void SetTimeOfDayManagerGameplayOverride(UE4::UObject* InFortGameModeAthena, UE4::UObject* InTimeOfDayManagerClass)
		{
			UE4::UObject* TimeOfDayManagerClass = InTimeOfDayManagerClass;
			UE4::ProcessEvent(InFortGameModeAthena, Offsets::SetTimeOfDayManagerGameplayOverrideFunc, &TimeOfDayManagerClass, 0);
		}

		static UE4::UObject* GetCurrentWeapon(UE4::UObject* InFortPawn)
		{
			return *reinterpret_cast<UE4::UObject**>(__int64(InFortPawn) + __int64(Offsets::CurrentWeaponOffset));
		}

		static UE4::UObject* GetWeaponDefinitionFromAPID(UE4::UObject* InAPID) // APID is short for AthenaPickaxeItemDefinition
		{
			return *reinterpret_cast<UE4::UObject**>(__int64(InAPID) + __int64(Offsets::WeaponDefinitionOffset));
		}

		static bool IsTargettingWeapon(UE4::UObject* InFortWeapon)
		{
			return *reinterpret_cast<bool*>(__int64(InFortWeapon) + __int64(Offsets::bIsTargetingOffset));
		}

		static void ServerPlayEmoteItem(UE4::UObject* InController, UE4::UObject* InEmoteAsset, float InEmoteRandomNumber)
		{
			struct
			{
				UE4::UObject* EmoteAsset;
				float EmoteRandomNumber;
			} ServerPlayEmoteItemParams;
			ServerPlayEmoteItemParams.EmoteAsset = InEmoteAsset;
			ServerPlayEmoteItemParams.EmoteRandomNumber = InEmoteRandomNumber;
			UE4::ProcessEvent(InController, Offsets::ServerPlayEmoteItemFunc, &ServerPlayEmoteItemParams, 0);
		};

		static void PlayEmoteItem(UE4::UObject* InController, UE4::UObject* InEmoteAsset, uint8_t InPlayMode)
		{
			struct
			{
				UE4::UObject* EmoteAsset;
				uint8_t PlayMode;
			} PlayEmoteItemParams;

			PlayEmoteItemParams.EmoteAsset = InEmoteAsset;
			PlayEmoteItemParams.PlayMode = InPlayMode;

			UE4::ProcessEvent(InController, Offsets::PlayEmoteItemFunc, &PlayEmoteItemParams, 0);
		};

		static UE4::UObject* GetAnimationHardReference(UE4::UObject* InFortMontageItemDefinitionBase, uint8_t InBodyType, uint8_t InGender, UE4::UObject* InPawnContext)
		{
			struct
			{
				uint8_t BodyType; //EFortCustomBodyType
				uint8_t Gender; //EFortCustomGender
				UE4::UObject* PawnContext;
				UE4::UObject* ReturnValue;
			} GetAnimationHardReferenceParams;
			GetAnimationHardReferenceParams.BodyType = InBodyType;
			GetAnimationHardReferenceParams.Gender = InGender;
			GetAnimationHardReferenceParams.PawnContext = InPawnContext;
			UE4::ProcessEvent(InFortMontageItemDefinitionBase, Offsets::GetAnimationHardReferenceFunc, &GetAnimationHardReferenceParams, 0);
			return GetAnimationHardReferenceParams.ReturnValue;
		}

		static void Montage_Stop(UE4::UObject* InAnimInstance, float BlendOutTime, UE4::UObject* InAnimMontage)
		{
			struct
			{
				float InBlendOutTime;
				UE4::UObject* Montage;
			} Montage_StopParams;
			Montage_StopParams.InBlendOutTime = BlendOutTime;
			Montage_StopParams.Montage = InAnimMontage;
			UE4::ProcessEvent(InAnimInstance, Offsets::Montage_StopFunc, &Montage_StopParams, 0);
		}

		static void PlayLocalAnimMontage(UE4::UObject* InFortPawn, UE4::UObject* InNewAnimMontage)
		{
			struct
			{
				UE4::UObject* NewAnimMontage;
				float InPlayRate;
				UE4::FName StartSectionName;
				bool bNoBlend;
				float ReturnValue;
			} PlayLocalAnimMontageParams;

			PlayLocalAnimMontageParams.NewAnimMontage = InNewAnimMontage;
			PlayLocalAnimMontageParams.InPlayRate = 1.0f;
			PlayLocalAnimMontageParams.StartSectionName.ComparisonIndex = 0;
			PlayLocalAnimMontageParams.StartSectionName.Number = 0;
			PlayLocalAnimMontageParams.bNoBlend = false;
			UE4::ProcessEvent(InFortPawn, Offsets::PlayLocalAnimMontageFunc, &PlayLocalAnimMontageParams, 0);
		}

		static UE4::UObject* GetCharacterSkletalMeshComponent(UE4::UObject* InCharacter)
		{
			return *reinterpret_cast<UE4::UObject**>(__int64(InCharacter) + __int64(Offsets::CharacterMeshOffset));
		}

		static UE4::UObject* GetCurrentMontage(UE4::UObject* InCharacter)
		{
			UE4::UObject* ReturnValue;
			UE4::ProcessEvent(InCharacter, Offsets::GetCurrentMontageFunc, &ReturnValue, 0);
			return ReturnValue;
		}

		static UE4::UObject* GetAnimInstance(UE4::UObject* InSkeletalMeshComponent)
		{
			UE4::UObject* ReturnValue;
			UE4::ProcessEvent(InSkeletalMeshComponent, Offsets::GetAnimInstanceFunc, &ReturnValue, 0);
			return ReturnValue;
		}

		static void StopEmoting()
		{
			CurrentEmote = nullptr;
			UE4::UObject* SkeletalMeshComponent = RiftAutomationUtils::GetCharacterSkletalMeshComponent(Pawn);
			UE4::UObject* AnimInstance = RiftAutomationUtils::GetAnimInstance(SkeletalMeshComponent);
			RiftAutomationUtils::Montage_Stop(AnimInstance, 0.1f, CurrentEmote);
		}

		static uint8_t GetCharacterGender(UE4::UObject* InFortPlayerState)
		{
			return *reinterpret_cast<uint8_t*>(__int64(InFortPlayerState) + __int64(Offsets::CharacterGenderOffset));
		}

		static uint8_t GetCharacterBodyType(UE4::UObject* InFortPlayerState)
		{
			return *reinterpret_cast<uint8_t*>(__int64(InFortPlayerState) + __int64(Offsets::CharacterBodyTypeOffset));
		}

		static bool IsFalling(UE4::UObject* InNavMovementComponent)
		{
			bool ReturnValue;
			UE4::ProcessEvent(InNavMovementComponent, Offsets::IsFallingFunc, &ReturnValue, 0);
			return ReturnValue;
		}

		static UE4::UObject* GetRuntimeOptions()
		{
			UE4::UObject* ReturnValue;
			UE4::ProcessEvent(Offsets::Default__FortRuntimeOptions, Offsets::GetRuntimeOptionsFunc, &ReturnValue, 0);
			return ReturnValue;
		}

		static UE4::FString GetDefaultFlagRegionId(UE4::UObject* InFortRuntimeOptions)
		{
			return *reinterpret_cast<UE4::FString*>(__int64(InFortRuntimeOptions) + __int64(Offsets::DefaultFlagRegionIdOffset));
		}

		static int GetMaxNumItemsInCreativeChests(UE4::UObject* InFortRuntimeOptions)
		{
			return *reinterpret_cast<int*>(__int64(InFortRuntimeOptions) + __int64(Offsets::MaxNumItemsInCreativeChestsOffset));
		}

		static void UnCrouch(UE4::UObject* InCharacter, bool ClientSimulation = false)
		{
			UE4::ProcessEvent(InCharacter, Offsets::UnCrouchFunc, &ClientSimulation, 0);
		}
	};


	class RiftEventUtils
	{
	public:
		static void WatchNewYears()
		{
			auto NYETimer = GlobalObjects->FindObjectByFullName(skCrypt("BP_NewYearTimer_C /NewYears/Levels/Apollo_NYE_Celebration.Apollo_NYE_Celebration.PersistentLevel.BP_NewYearTimer_2"));

			if (NYETimer)
			{
				auto InitializeLevel = GlobalObjects->FindObjectByFullName(skCrypt("Function /NewYears/Blueprints/BP_NewYearTimer.BP_NewYearTimer_C.InitializeLevel"));
				auto StartNYE = GlobalObjects->FindObjectByFullName(skCrypt("Function /NewYears/Blueprints/BP_NewYearTimer.BP_NewYearTimer_C.startNYE"));

				UE4::ProcessEvent(NYETimer, InitializeLevel, nullptr, 0);
				UE4::ProcessEvent(NYETimer, StartNYE, nullptr, 0);
			}
		}
	};

	static void ExecutePatches()
	{
#pragma warning( push )
#pragma warning( disable : 4309 )

		if (UE4::SprintPatchAddr)
		{
			*(char*)(UE4::SprintPatchAddr + 0x0) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x1) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x2) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x3) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x4) = 0x90;
			*(char*)(UE4::SprintPatchAddr + 0x5) = 0x90;
		}
		else
			DEBUG_LOG("Sprint patch address was NULL!\n");

		if (UE4::WeaponPatchAddr)
		{
			*(char*)(UE4::WeaponPatchAddr + 0x1) = 0x85;
			*(char*)(UE4::WeaponPatchAddr + 0xA) = 0x8D;
		}
		else
			DEBUG_LOG("Weapon patch address was NULL!\n");

#pragma warning( pop )
	}

	static void CrashGame() //pretty dumb and *unpredictable* way of crashing the game
	{
		*(char*)(UE4::ProcessEventAddr) = 0x0;
		*(char*)(UE4::ProcessEventAddr + 0x1) = 0x0;

		*(char*)(UE4::SpawnActorAddr) = 0x0;
		*(char*)(UE4::SpawnActorAddr + 0x1) = 0x0;

		*(char*)(UE4::StaticConstructObject_InternalAddr) = 0x0;
		*(char*)(UE4::StaticConstructObject_InternalAddr + 0x1) = 0x0;
		
		*(char*)(UE4::GetNameByIndexAddr) = 0x0;
		*(char*)(UE4::GetNameByIndexAddr + 0x1) = 0x0;
	}

	static void GetOffsets()
	{
		Offsets::CheatManagerOffset = GlobalObjects->FindOffset(skCrypt("PlayerController"), skCrypt("CheatManager"));
		Offsets::CurrentMovementStyleOffset = GlobalObjects->FindOffset(skCrypt("FortPawn"), skCrypt("CurrentMovementStyle"));
		Offsets::CharacterMovementOffset = GlobalObjects->FindOffset(skCrypt("Character"), skCrypt("CharacterMovement"));
		Offsets::bInfiniteAmmoOffset = GlobalObjects->FindOffset(skCrypt("FortPlayerController"), skCrypt("bInfiniteAmmo"));
		Offsets::CurrentPlaylistInfoOffset = GlobalObjects->FindOffset(skCrypt("FortGameStateAthena"), skCrypt("CurrentPlaylistInfo"));
		Offsets::BasePlaylistOffset = GlobalObjects->FindOffset(skCrypt("PlaylistPropertyArray"), skCrypt("BasePlaylist"));
		Offsets::bWantsToSprintOffset = GlobalObjects->FindOffset(skCrypt("FortPlayerController"), skCrypt("bWantsToSprint"));
		Offsets::bSkipWarmupOffset = GlobalObjects->FindOffset(skCrypt("FortPlaylistAthena"), skCrypt("bSkipWarmup"));
		Offsets::bSkipAircraftOffset = GlobalObjects->FindOffset(skCrypt("FortPlaylistAthena"), skCrypt("bSkipAircraft"));
		Offsets::McpProfileGroupOffset = GlobalObjects->FindOffset(skCrypt("FortPlayerController"), skCrypt("McpProfileGroup"));
		Offsets::PlayerNameOffset = GlobalObjects->FindOffset(skCrypt("McpProfileGroup"), skCrypt("PlayerName"));
		Offsets::CurrentWeaponOffset = GlobalObjects->FindOffset(skCrypt("FortPawn"), skCrypt("CurrentWeapon"));
		Offsets::bIsTargetingOffset = GlobalObjects->FindOffset(skCrypt("FortWeapon"), skCrypt("bIsTargeting"));
		Offsets::PawnPlayerStateOffset = GlobalObjects->FindOffset(skCrypt("Controller"), skCrypt("PlayerState"));
		Offsets::WeaponDefinitionOffset = GlobalObjects->FindOffset(skCrypt("AthenaPickaxeItemDefinition"), skCrypt("WeaponDefinition"));
		Offsets::CharacterMeshOffset = GlobalObjects->FindOffset(skCrypt("Character"), skCrypt("Mesh"));
		Offsets::CharacterGenderOffset = GlobalObjects->FindOffset(skCrypt("FortPlayerState"), skCrypt("CharacterGender"));
		Offsets::CharacterBodyTypeOffset = GlobalObjects->FindOffset(skCrypt("FortPlayerState"), skCrypt("CharacterBodyType"));
		Offsets::MaxNumItemsInCreativeChestsOffset = GlobalObjects->FindOffset(skCrypt("FortRuntimeOptions"), skCrypt("MaxNumItemsInCreativeChests"));
		Offsets::DefaultFlagRegionIdOffset = GlobalObjects->FindOffset(skCrypt("FortRuntimeOptions"), skCrypt("DefaultFlagRegionId"));


		Offsets::CheatManagerClass = GlobalObjects->FindObjectByFullName(skCrypt("Class /Script/Engine.CheatManager"));

		Offsets::Default__FortKismetLibrary = GlobalObjects->FindObjectByFullName(skCrypt("FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary"));
		Offsets::Default__FortItemAndVariantSwapHelpers = GlobalObjects->FindObjectByFullName(skCrypt("FortItemAndVariantSwapHelpers /Script/FortniteGame.Default__FortItemAndVariantSwapHelpers"));
		Offsets::Default__FortRuntimeOptions = GlobalObjects->FindObjectByFullName(skCrypt("FortRuntimeOptions /Script/FortniteGame.Default__FortRuntimeOptions"));

		Offsets::UnCrouchFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.Character.UnCrouch"));
		Offsets::CrouchFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.Character.Crouch"));
		Offsets::GetRuntimeOptionsFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortRuntimeOptions.GetRuntimeOptions"));
		Offsets::PlayerPawnAthenaTickFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C.ReceiveTick"));
		Offsets::IsFallingFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.NavMovementComponent.IsFalling"));
		Offsets::GetCurrentMontageFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.Character.GetCurrentMontage"));
		Offsets::GetAnimInstanceFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.SkeletalMeshComponent.GetAnimInstance"));
		Offsets::Montage_StopFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.AnimInstance.Montage_Stop"));
		Offsets::PlayLocalAnimMontageFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPawn.PlayLocalAnimMontage"));
		Offsets::GetAnimationHardReferenceFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortMontageItemDefinitionBase.GetAnimationHardReference"));
		Offsets::PlayEmoteItemFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerController.PlayEmoteItem"));
		Offsets::ServerPlayEmoteItemFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerController.ServerPlayEmoteItem"));
		Offsets::SetTimeOfDayManagerGameplayOverrideFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortGameModeAthena.SetTimeOfDayManagerGameplayOverride"));
		Offsets::BugItGoFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.CheatManager.BugItGo"));
		Offsets::GetActorLocationFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.Actor.K2_GetActorLocation"));
		Offsets::JumpFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.Character.Jump"));
		Offsets::JumpToSafeZonePhaseFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortGameModeAthena.JumpToSafeZonePhase"));
		Offsets::OnRep_CurrentPlaylistInfoFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortGameStateAthena.OnRep_CurrentPlaylistInfo"));
		Offsets::TeleportToSkyDiveFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerPawnAthena.TeleportToSkyDive"));
		Offsets::EquipWeaponDefinitionFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPawn.EquipWeaponDefinition"));
		Offsets::PushCosmeticOverrideOntoPawnFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortItemAndVariantSwapHelpers.PushCosmeticOverrideOntoPawn"));
		Offsets::SetCanBeDamagedFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortKismetLibrary.SetCanBeDamaged"));
		Offsets::DestroyAllFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.CheatManager.DestroyAll"));
		Offsets::StartMatchFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.GameMode.StartMatch"));
		Offsets::ServerReadyToStartMatchFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerController.ServerReadyToStartMatch"));
		Offsets::PossessFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.Controller.Possess"));
		Offsets::PlayButtonFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Game/Athena/UI/Matchmaking/AthenaLobbyMatchmakingPlay.AthenaLobbyMatchmakingPlay_C.BndEvt__PlayButton_K2Node_ComponentBoundEvent_0_CommonButtonClicked__DelegateSignature"));
		Offsets::ReadyToStartMatchFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.GameMode.ReadyToStartMatch"));
		Offsets::SummonFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.CheatManager.Summon"));
		Offsets::SwitchLevelFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.PlayerController.SwitchLevel"));
		Offsets::IsSkydivingFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerPawn.IsSkydiving"));
		Offsets::IsParachuteOpenFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerPawn.IsParachuteOpen"));
		Offsets::IsParachuteForcedOpenFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerPawn.IsParachuteForcedOpen"));
		Offsets::SetMovementModeFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/Engine.CharacterMovementComponent.SetMovementMode"));
		Offsets::OnRep_IsParachuteOpenFunc = GlobalObjects->FindObjectByFullName(skCrypt("Function /Script/FortniteGame.FortPlayerPawn.OnRep_IsParachuteOpen"));
	}

	bool bIsInGame = false;

	static void LoadAthena()
	{
		using namespace UE4;
		Controller = (UObject*)(UE4::GetFirstPlayerController(*GWorld));

		CheatManager = RiftAutomationUtils::CreateCheatManager(Controller);
		RiftAutomationUtils::BugItGo(CheatManager, 0, 0, 0, 0, 0, 0);
		RiftAutomationUtils::Summon(CheatManager, skCrypt("PlayerPawn_Athena_C"));
		
		DEBUG_LOG("PlayerPawn has been Summoned!\n");

		Pawn = GlobalObjects->FindObject(skCrypt("PlayerPawn_Athena_C_"));
		GameState = GlobalObjects->FindObject(skCrypt("Athena_GameState_C_")); //ENCRYPT THESE.
		GameMode = GlobalObjects->FindObject(skCrypt("Athena_GameMode_C_"));
		PlayerState = RiftAutomationUtils::GetPlayerState(Controller);
		CharacterMovement = RiftAutomationUtils::GetCharacterMovementComponent(Pawn);

		DEBUG_LOG("Controller: %p : %s\n", Controller, Controller->GetFullName());
		DEBUG_LOG("Pawn: %p : %s\n", Pawn, Pawn->GetFullName());
		DEBUG_LOG("GameState: %s\n", GameState->GetFullName());
		DEBUG_LOG("GameMode: %s\n", GameMode->GetFullName()); 
		DEBUG_LOG("PlayerState: %s\n", PlayerState->GetFullName());
		DEBUG_LOG("CharacterMovement: %s\n", CharacterMovement->GetFullName());

		//UE4::UObject* NewTODM = UE4::StaticLoadObject(GlobalObjects->FindObjectByFullName("Class /Script/Engine.BlueprintGeneratedClass"), nullptr, L"/Game/TimeOfDay/TODM/STW/Children/TODM_STW_Winter_2018.TODM_STW_Winter_2018_C");
		//RiftAutomationUtils::SetTimeOfDayManagerGameplayOverride(GameMode, NewTODM);

		RiftAutomationUtils::SetPlaylist(GameState, skCrypt("FortPlaylistAthena /Game/Athena/Playlists/Showdown/Playlist_ShowdownAlt_Solo.Playlist_ShowdownAlt_Solo"), true, true);
		DEBUG_LOG("SetPlaylist\n");

		RiftAutomationUtils::Possess(Controller, Pawn);
		DEBUG_LOG("Possess\n");

		RiftAutomationUtils::JumpToSafeZonePhase(GameMode);
		DEBUG_LOG("JumpToSafeZonePhase\n");

		/*
		RiftAutomationUtils::OverridePawnCosmetic(Pawn, skCrypt("AthenaCharacterItemDefinition /Game/Heroes/Commando/CosmeticCharacterItemDefinitions/CID_Commando_016_F_V1.CID_Commando_016_F_V1"));
		DEBUG_LOG("OverridePawnCosmetic\n");
		*/

		std::string AccountName = RiftAutomationUtils::GetAccountName();
		DEBUG_LOG("Player Name: %s\n", AccountName.c_str());

		/*
		nlohmann::json PlayerLoadout = GetLoadoutFromAPI(AccountName);

		bool DoesGliderExist = !(PlayerLoadout["glider"].is_null());
		if (DoesGliderExist)
		{
			DEBUG_LOG("Glider DOES indeed exist in response!\n");
			std::string GliderName = PlayerLoadout["glider"].get<std::string>();
			DEBUG_LOG(GliderName.c_str());
			RiftAutomationUtils::OverridePawnCosmetic(Pawn, GliderName.c_str(), true);
		}
		else
			DEBUG_LOG("API down? Or glider is invalid!");

		bool DoesPickaxeExist = !(PlayerLoadout["pickaxe"].is_null());
		if (DoesPickaxeExist)
		{
			DEBUG_LOG("Pickaxe DOES indeed exist in response!\n");
			std::string PickaxeName = PlayerLoadout["pickaxe"].get<std::string>();
			DEBUG_LOG(PickaxeName.c_str());
			UE4::UObject* APID = GlobalObjects->FindObjectByFullNameInsensitive(PickaxeName.c_str());
			UE4::UObject* WeaponDef = RiftAutomationUtils::GetWeaponDefinitionFromAPID(APID);
			if (WeaponDef)
				RiftAutomationUtils::EquipWeaponFromDefinition(Pawn, WeaponDef);
			else
				DEBUG_LOG("Invalid WeaponDefinition grabbed from AGID!");
		}
		bool DoesContrailExist = !(PlayerLoadout["contrail"].is_null());
		if (DoesContrailExist)
		{
			DEBUG_LOG("Contrail DOES indeed exist in response!\n");
			std::string ContrailName = PlayerLoadout["contrail"].get<std::string>();
			DEBUG_LOG(ContrailName.c_str());
			RiftAutomationUtils::OverridePawnCosmetic(Pawn, ContrailName.c_str(), true);
		}
		*/

		RiftAutomationUtils::TeleportToSkyDive(Pawn, 100000);
		DEBUG_LOG("TeleportToSkyDive\n");

		RiftAutomationUtils::ServerReadyToStartMatch(Controller);
		DEBUG_LOG("ServerReadyToStartMatch\n");

		RiftAutomationUtils::StartMatch(GameMode);
		DEBUG_LOG("StartMatch\n");

		RiftAutomationUtils::DestroyAll(CheatManager, skCrypt("FortHLODSMActor"));
		DEBUG_LOG("DestroyAll\n");

		RiftAutomationUtils::SetActorGodMode(Pawn, true);
		DEBUG_LOG("GiveActorGodMode\n");

		RiftAutomationUtils::SetInfiniteAmmo(Controller);
		DEBUG_LOG("SetInfiniteAmmo\n");

		//RiftAutomationUtils::EquipWeapon(Pawn, skCrypt("FortWeaponMeleeItemDefinition /Mantis/Items/UncleBrolly/WID_UncleBrolly_VR.WID_UncleBrolly_VR"));
		DEBUG_LOG("EquipWeapon\n");

		ExecutePatches();
		bIsInGame = true;  //Figure out a way to do this as loading screen drops.
	}


	typedef void(__fastcall* fCrouchInternal)(UE4::UObject*, bool);
	static fCrouchInternal CrouchInternal;

	static void CrouchInternalHook(UE4::UObject* InCharacter, bool bClientSimulation)
	{
		if (bIsInGame && CurrentEmote)
		{
			return;
		}
		return CrouchInternal(InCharacter, bClientSimulation);
	}

	typedef void (__fastcall* fPlayEmoteItemInternal)(UE4::UObject*, UE4::UObject*, uint8_t);
	static fPlayEmoteItemInternal PlayEmoteItemInternal;

	static void PlayEmoteItemHook(UE4::UObject* PlayerController, UE4::UObject* EmoteAsset, uint8_t PlayMode)
	{
		if (bIsInGame)
		{
			DEBUG_LOG("Emote Asset: %s", EmoteAsset->GetName());
			UE4::UObject* NewEmote = RiftAutomationUtils::GetAnimationHardReference(EmoteAsset, RiftAutomationUtils::GetCharacterBodyType(PlayerState), RiftAutomationUtils::GetCharacterGender(PlayerState), Pawn);
			if (NewEmote && NewEmote != CurrentEmote)
			{
				CurrentEmote = NewEmote;
				bool IsPlayerFalling = RiftAutomationUtils::IsFalling(CharacterMovement);
				if (!IsPlayerFalling)
				{
					UE4::FVector PlayerPos = RiftAutomationUtils::GetActorLocation(Pawn);
					CurrentEmotePositon = PlayerPos;
					RiftAutomationUtils::PlayLocalAnimMontage(Pawn, CurrentEmote);
					RiftAutomationUtils::UnCrouch(Pawn);
				}
			}
		}
		return PlayEmoteItemInternal(PlayerController, EmoteAsset, PlayMode);
	}

	typedef void* (__fastcall* fProcessEvent)(UE4::UObject* Object, UE4::UObject* Function, void* Params);
	static fProcessEvent ProcessEvent;

	bool ShouldOpenParachute = false;
	bool ShouldCloseParachute = false;

	static void* ProcessEventHook(UE4::UObject* Object, UE4::UObject* Function, void* Params)
	{
		if (Function == Offsets::PlayButtonFunc)
		{
			DEBUG_LOG("Play button was clicked: %s\n", Object->GetFullName());
			UE4::UObject* RuntimeOptions = RiftAutomationUtils::GetRuntimeOptions();
			std::string ImposterCheckOne = RiftAutomationUtils::GetDefaultFlagRegionId(RuntimeOptions).ToString();
			int ImposterCheckTwo = RiftAutomationUtils::GetMaxNumItemsInCreativeChests(RuntimeOptions);
			auto KismetDefaultFlagRegionId = skCrypt("Rift");
			if (ImposterCheckOne == KismetDefaultFlagRegionId.decrypt() && ImposterCheckTwo == 3945)
			{
				KismetDefaultFlagRegionId.clear();
				RiftAutomationUtils::SwitchLevel((UE4::UObject*)(UE4::GetFirstPlayerController(*GWorld)), skCrypt("Apollo_Terrain"));
			}
			else
			{
				KismetDefaultFlagRegionId.clear();
				DEBUG_LOG("RuntimeOptions mismatch, crashing the game!");
				CrashGame();
			}

		}

		else if (Function == Offsets::ReadyToStartMatchFunc)
		{
			std::string ObjName = Object->GetName();
			if (ObjName.find(skCrypt("Athena_GameMode_C_")) != std::string::npos)
			{
				DEBUG_LOG("Ready to start match: %s\n", Object->GetFullName());
				LoadAthena();
			}
		}

		if (ShouldOpenParachute)
		{
			ShouldOpenParachute = false;
			RiftAutomationUtils::SetMovementMode(CharacterMovement, 6, 3U);
		}

		if (ShouldCloseParachute)
		{
			ShouldCloseParachute = false;
			RiftAutomationUtils::SetMovementMode(CharacterMovement, 6, 4U);
		}

		return ProcessEvent(Object, Function, Params);
	}

	bool StopHoldingKey = false;

	DWORD RiftPlayerPawnTick(LPVOID lpParam)
	{
		while (true)
		{
			if (bIsInGame)
			{
				if (RiftAutomationUtils::WantsToSprint(Controller))
					RiftAutomationUtils::SetCurrentMovementStyle(Pawn, 3);
				else
					RiftAutomationUtils::SetCurrentMovementStyle(Pawn, 0);

				if (CurrentEmote)
				{
					UE4::FVector PawnPos = RiftAutomationUtils::GetActorLocation(Pawn);
					float Xdif = PawnPos.X - CurrentEmotePositon.X;
					float Ydif = PawnPos.Y - CurrentEmotePositon.Y;
					if (Xdif > 75 || Xdif < -75 || Ydif > 75 || Ydif < -75)
						RiftAutomationUtils::StopEmoting();

					bool IsPlayerFalling = RiftAutomationUtils::IsFalling(CharacterMovement);
					if (IsPlayerFalling)
					{
						RiftAutomationUtils::StopEmoting();
					}
				}



				if (GetAsyncKeyState(VK_SPACE) & 0x8000)
				{
					if (StopHoldingKey == false)
					{
						StopHoldingKey = true;

						bool bIsSkydiving = RiftAutomationUtils::IsSkydiving(Pawn);
						bool bIsParachuteOpen = RiftAutomationUtils::IsParachuteOpen(Pawn);
						bool bIsParachuteForcedOpen = RiftAutomationUtils::IsParachuteForcedOpen(Pawn);
						if (bIsSkydiving && !ShouldOpenParachute && !ShouldCloseParachute)
						{
							if (bIsSkydiving && !bIsParachuteOpen && !bIsParachuteForcedOpen)
								ShouldOpenParachute = true;

							else if (bIsParachuteOpen && !bIsParachuteForcedOpen)
								ShouldCloseParachute = true;
						}
						else
						{
							if (CurrentEmote)
								RiftAutomationUtils::StopEmoting();

							RiftAutomationUtils::Jump(Pawn);
						}
					}
				}

				else if (GetAsyncKeyState(VK_F1) & 0x8000)
				{
					if (StopHoldingKey == false)
					{
						StopHoldingKey = true;

						bool bIsSkydiving = RiftAutomationUtils::IsSkydiving(Pawn);
						if (!bIsSkydiving) //Game crashes if you have glider out and call TeleportToSkyDive, might aswell have it so this keybind only works when your on the ground.
						{
							UE4::FVector PawnPos = RiftAutomationUtils::GetActorLocation(Pawn);
							if (PawnPos.Z < 25000)
							{
								RiftAutomationUtils::StopEmoting();
								RiftAutomationUtils::TeleportToSkyDive(Pawn, 100000);
							}
						}
					}
				}

				else if (GetAsyncKeyState(VK_F3) & 0x8000)
				{
					if (StopHoldingKey == false)
					{
						StopHoldingKey = true;
						bIsInGame = false;
						RiftAutomationUtils::StopEmoting();
						RiftAutomationUtils::SwitchLevel((UE4::UObject*)(UE4::GetFirstPlayerController(*GWorld)), skCrypt("Frontend"));
					}
				}

				else
					StopHoldingKey = false;
			}
			Sleep(1000 / 60);
		}
	}

	static bool PostInit()
	{
		DEBUG_LOG("Rift PostInit has begun!\n");

		GetOffsets();

		ProcessEvent = (fProcessEvent)(UE4::ProcessEventAddr);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)ProcessEvent, ProcessEventHook);

		if (UE4::PlayEmoteItemInternalAddr)
		{
			PlayEmoteItemInternal = (fPlayEmoteItemInternal)(UE4::PlayEmoteItemInternalAddr);
			DetourAttach(&(PVOID&)PlayEmoteItemInternal, PlayEmoteItemHook);
		}
		else
			DEBUG_LOG("PlayEmoteItem address is null, emoting will no longer work.");

		if (UE4::CrouchAddr)
		{
			CrouchInternal = (fCrouchInternal)(UE4::CrouchAddr);
			DetourAttach(&(PVOID&)CrouchInternal, CrouchInternalHook);
		}
		else
			DEBUG_LOG("Crouch address is null, emote crouch bug will be in effect.");

		DetourTransactionCommit();
		CreateThread(0, 0, RiftPlayerPawnTick, 0, 0, 0);

		return true;
	};

	DWORD StartupGWorldCheckLoop(LPVOID lpParam)
	{
		bool bCheckGWorldOnce = true;
		bool bShouldLoop = true;

		if (bCheckGWorldOnce)
		{
			if (*GWorld) // GWorld should NOT be existant on injection, most likely means someones trying to skid Rift.
			{
				bCheckGWorldOnce = false;
				DEBUG_LOG("Rift should not be injected post-GWorld");
				CrashGame();
			}
			bCheckGWorldOnce = false;
		}

		while (bShouldLoop)
		{
			if (*GWorld)
			{
				UE4::UObject* AreOffsetsAvailable = GlobalObjects->FindObject(skCrypt("BndEvt__PlayButton_K2Node_ComponentBoundEvent_0_CommonButtonClicked__DelegateSignature"));
				if (AreOffsetsAvailable)
				{
					bShouldLoop = false;
					PostInit();
				}
			}
			Sleep(200);
		}
		return NULL;
	}

	static bool Init()
	{
		GWorld = reinterpret_cast<UE4::UObject**>(Memory::FindPattern(skCrypt(GWORLD_PATTERN_ONE), true, 3));
		if (!GWorld)
			GWorld = reinterpret_cast<UE4::UObject**>(Memory::FindPattern(skCrypt(GWORLD_PATTERN_TWO), true, 3));

		UE4::GObjectsAddr = Memory::FindPattern(skCrypt(GOBJECT_PATTERN), true, 10);
		UE4::FreeAddr = Memory::FindPattern(skCrypt(FMEMORYFREE_PATTERN));
		UE4::GetObjNameAddr = Memory::FindPattern(skCrypt(GETOBJNAME_PATTERN));
		UE4::GetFirstPlayerControllerAddr = Memory::FindPattern(skCrypt(GETFIRSTPLAYERCONTROLLER_PATTERN));
		UE4::ProcessEventAddr = Memory::FindPattern(skCrypt(PROCESSEVENT_PATTERN));
		UE4::StaticConstructObject_InternalAddr = Memory::FindPattern(skCrypt(STATICCONSTRUCTOBJECTINTERNAL_PATTERN));
		UE4::GetNameByIndexAddr = Memory::FindPattern(skCrypt(GETNAMEBYINDEX_PATTERN));
		UE4::StaticLoadObjectAddr = Memory::FindPattern(skCrypt(STATICLOADOBJECT_PATTERN)); //Not needed, if failed to find, don't worry.
		UE4::SpawnActorAddr = Memory::FindPattern(skCrypt(SPAWNACTOR_PATTERN));
		UE4::SprintPatchAddr = Memory::FindPattern(skCrypt(SPRINTPATCH_PATTERN));
		UE4::WeaponPatchAddr = Memory::FindPattern(skCrypt(WEAPONPATCH_PATTERN));
		UE4::PlayEmoteItemInternalAddr = Memory::FindPattern(skCrypt(PLAYEMOTEITEM_PATTERN));
		UE4::CrouchAddr = Memory::FindPattern(skCrypt(CROUCH_PATTERN)); //Not essential for for core gameplay, dont worry if failed to find

		if (!UE4::GObjectsAddr || !UE4::FreeAddr || !UE4::GetObjNameAddr || !UE4::GetFirstPlayerControllerAddr || !UE4::ProcessEventAddr || !UE4::StaticConstructObject_InternalAddr || !GWorld || !UE4::GetNameByIndexAddr)
		{
			if (!UE4::GObjectsAddr)
				DEBUG_LOG("Failed to find GObjects.\n");
			if (!UE4::FreeAddr)
				DEBUG_LOG("Failed to find Free.\n");
			if (!UE4::GetObjNameAddr)
				DEBUG_LOG("Failed to find GetObjectName.\n");
			if (!UE4::ProcessEventAddr)
				DEBUG_LOG("Failed to find ProcessEvent.\n");
			if (!UE4::StaticConstructObject_InternalAddr)
				DEBUG_LOG("Failed to find StaticConstructObject_Internal.\n");
			if (!GWorld)
				DEBUG_LOG("Failed to find GWorld.\n");
			if (!UE4::GetNameByIndexAddr)
				DEBUG_LOG("Failed to find GetNameByIndex.\n");

			DEBUG_LOG("One or more patterns was incorrect.\n");
			return false;
		}

		GlobalObjects = reinterpret_cast<UE4::GObjects*>(UE4::GObjectsAddr);

		std::cout << GlobalObjects->GetByIndex(9) << std::endl;

		CreateThread(0, 0, StartupGWorldCheckLoop, 0, 0, 0);
		return true;
	}
};