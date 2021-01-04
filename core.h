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

		UE4::UObject* CheatManagerClass;

		UE4::UObject* Default__FortKismetLibrary;
		UE4::UObject* Default__FortItemAndVariantSwapHelpers;

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
	};

	static void ExecutePatches()
	{
		//Sprinting Patch
		*(char*)(UE4::SprintPatchAddr + 0x0) = 0x90;
		*(char*)(UE4::SprintPatchAddr + 0x1) = 0x90;
		*(char*)(UE4::SprintPatchAddr + 0x2) = 0x90;
		*(char*)(UE4::SprintPatchAddr + 0x3) = 0x90;
		*(char*)(UE4::SprintPatchAddr + 0x4) = 0x90;
		*(char*)(UE4::SprintPatchAddr + 0x5) = 0x90;

		//Weapon Patch
		*(char*)(UE4::WeaponPatchAddr + 0x1) = 0x85;
		*(char*)(UE4::WeaponPatchAddr + 0xA) = 0x8D;
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
		Offsets::WeaponDefinitionOffset = GlobalObjects->FindOffset("AthenaPickaxeItemDefinition", "WeaponDefinition");

		Offsets::CheatManagerClass = GlobalObjects->FindObjectByFullName(skCrypt("Class /Script/Engine.CheatManager"));

		Offsets::Default__FortKismetLibrary = GlobalObjects->FindObjectByFullName(skCrypt("FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary"));
		Offsets::Default__FortItemAndVariantSwapHelpers = GlobalObjects->FindObjectByFullName(skCrypt("FortItemAndVariantSwapHelpers /Script/FortniteGame.Default__FortItemAndVariantSwapHelpers"));
		
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

		DEBUG_LOG("Pawn: %s\n", Pawn->GetFullName());
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

		RiftAutomationUtils::OverridePawnCosmetic(Pawn, skCrypt("AthenaCharacterItemDefinition /Game/Heroes/Commando/CosmeticCharacterItemDefinitions/CID_Commando_016_F_V1.CID_Commando_016_F_V1"));
		DEBUG_LOG("OverridePawnCosmetic\n");

		std::string AccountName = RiftAutomationUtils::GetAccountName();
		DEBUG_LOG("Player Name: %s\n", AccountName.c_str());
		nlohmann::json PlayerLoadout = GetLoadoutFromAPI(AccountName);

		bool DoesGliderExist = !(PlayerLoadout["glider"].is_null());
		if (DoesGliderExist)
		{
			DEBUG_LOG("Glider DOES indeed exist in response!\n");
			std::string GliderName = PlayerLoadout["glider"].get<std::string>();
			std::cout << GliderName << std::endl;
			RiftAutomationUtils::OverridePawnCosmetic(Pawn, GliderName.c_str(), true);
		}
		else
			DEBUG_LOG("API down? Or glider is invalid!");

		bool DoesPickaxeExist = !(PlayerLoadout["pickaxe"].is_null());
		if (DoesPickaxeExist)
		{
			DEBUG_LOG("Pickaxe DOES indeed exist in response!\n");
			std::string PickaxeName = PlayerLoadout["pickaxe"].get<std::string>();
			std::cout << PickaxeName.c_str() << std::endl;
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
			std::cout << ContrailName.c_str() << std::endl;
			RiftAutomationUtils::OverridePawnCosmetic(Pawn, ContrailName.c_str(), true);
		}

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

	bool StopHoldingKey = false;
	typedef void* (__fastcall* fProcessEvent)(UE4::UObject* Object, UE4::UObject* Function, void* Params);
	static fProcessEvent ProcessEvent;
	static void* ProcessEventHook(UE4::UObject* Object, UE4::UObject* Function, void* Params)
	{
		if (Function == Offsets::PlayButtonFunc)
		{
			DEBUG_LOG("Play button was clicked: %s\n", Object->GetFullName());
			RiftAutomationUtils::SwitchLevel((UE4::UObject*)(UE4::GetFirstPlayerController(*GWorld)), skCrypt("Apollo_Terrain"));
		}

		else if (Function == Offsets::ReadyToStartMatchFunc)
		{
			std::string ObjName = Object->GetName();
			if (ObjName.find("Athena_GameMode_C_") != std::string::npos)
			{
				DEBUG_LOG("Ready to start match: %s\n", Object->GetFullName());
				LoadAthena();
			}
		}

		else if (bIsInGame)
		{
			if (Object == Pawn)
			{
				std::string FuncName = Function->GetName();
				if (FuncName.find("Tick") != std::string::npos) //should we skCrypt "Tick" ? idk
				{
					if (RiftAutomationUtils::WantsToSprint(Controller))
						RiftAutomationUtils::SetCurrentMovementStyle(Pawn, 3);
					else
						RiftAutomationUtils::SetCurrentMovementStyle(Pawn, 0);

					if (GetAsyncKeyState(VK_SPACE) & 0x8000)
					{
						if (StopHoldingKey == false)
						{
							StopHoldingKey = true;

							if (RiftAutomationUtils::IsSkydiving(Pawn))
							{
								if (RiftAutomationUtils::IsSkydiving(Pawn) && !RiftAutomationUtils::IsParachuteOpen(Pawn) && !RiftAutomationUtils::IsParachuteForcedOpen(Pawn))
								{
									RiftAutomationUtils::SetMovementMode(CharacterMovement, 6, 3U);
								}

								else if (RiftAutomationUtils::IsParachuteOpen(Pawn) && !RiftAutomationUtils::IsParachuteForcedOpen(Pawn))
								{
									RiftAutomationUtils::SetMovementMode(CharacterMovement, 6, 4U);
								}

								RiftAutomationUtils::OnRep_IsParachuteOpen(Pawn, RiftAutomationUtils::IsParachuteOpen(Pawn));
							}
							else
							{
								RiftAutomationUtils::Jump(Pawn);
							}
						}
					}
					else if (GetAsyncKeyState(VK_F5) & 0x8000) // Changing this from space to F5 due to the amount of times I've accidently gone back to lobby
					{
						bIsInGame = false;
						RiftAutomationUtils::SwitchLevel((UE4::UObject*)(UE4::GetFirstPlayerController(*GWorld)), skCrypt("Frontend"));
					}
					else if (GetAsyncKeyState(VK_F1) & 0x8000)
					{
						if (StopHoldingKey == false)
						{
							StopHoldingKey = true;
							UE4::FVector PawnPos = RiftAutomationUtils::GetActorLocation(Pawn);
							if (PawnPos.Z < 25000)
							{
								RiftAutomationUtils::TeleportToSkyDive(Pawn, 100000);
							}
						}
					}
					else
						StopHoldingKey = false;
				}
			}
		}
		return ProcessEvent(Object, Function, Params);
	}

	static bool PostInit()
	{
		DEBUG_LOG("Rift PostInit has begun!\n");

		GetOffsets();

		ProcessEvent = (fProcessEvent)(UE4::ProcessEventAddr);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)ProcessEvent, ProcessEventHook);
		DetourTransactionCommit();

		return true;
	};

	DWORD StartupGWorldCheckLoop(LPVOID lpParam)
	{
		bool bShouldLoop = true;
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
		GWorld = reinterpret_cast<UE4::UObject**>(Memory::FindPattern(skCrypt(GWORLD_PATTERN), true, 3));
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

		if (!UE4::GObjectsAddr || !UE4::FreeAddr || !UE4::GetObjNameAddr || !UE4::GetFirstPlayerControllerAddr || !UE4::ProcessEventAddr || !UE4::StaticConstructObject_InternalAddr || !GWorld || !UE4::GetNameByIndexAddr || !UE4::StaticLoadObjectAddr || !UE4::SprintPatchAddr || !UE4::WeaponPatchAddr)
		{
			DEBUG_LOG("One or more patterns was incorrect.\n");
			return false;
		}

		GlobalObjects = reinterpret_cast<UE4::GObjects*>(UE4::GObjectsAddr);

		CreateThread(0, 0, StartupGWorldCheckLoop, 0, 0, 0);
		return true;
	}
};