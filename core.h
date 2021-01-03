#pragma once
#include "memory.h"
#include "offsets.h"
#include "ue4.h"
#include "detours.h"

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

		UE4::UObject* CheatManagerClass;

		UE4::UObject* Default__FortKismetLibrary;
		UE4::UObject* Default__FortItemAndVariantSwapHelpers;

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
		static void Summon(UE4::UObject* InCheatManager, UE4::FString ClassToSummon) { UE4::ProcessEvent(InCheatManager, Offsets::SummonFunc, &ClassToSummon, 0); }

		static void Possess(UE4::UObject* InController, UE4::UObject* InPawn) { UE4::ProcessEvent(InController, Offsets::PossessFunc, &InPawn, 0); }

		static void ServerReadyToStartMatch(UE4::UObject* InController) { UE4::ProcessEvent(InController, Offsets::ServerReadyToStartMatchFunc, nullptr, 0); }

		static void StartMatch(UE4::UObject* InGameMode) { UE4::ProcessEvent(InGameMode, Offsets::StartMatchFunc, nullptr, 0); }

		static void DestroyAll(UE4::UObject* InCheatManager, const std::string& ClassToDestroy)
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

		static void OverridePawnCosmetic(UE4::UObject* InFortPlayerPawn, const std::string& CosmeticFullName)
		{
			auto Cosmetic = GlobalObjects->FindObjectByFullName(CosmeticFullName);
			if (!Cosmetic)
			{
				std::cout << "Invalid cosmetic name!" << std::endl;
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
		static void EquipWeapon(UE4::UObject* InPawn, const std::string& WIDFullName)
		{
			auto WeaponToEquip = GlobalObjects->FindObjectByFullName(WIDFullName);
			if (!WeaponToEquip)
			{
				std::cout << "Invalid weapon definition!" << std::endl;
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

		static void SetPlaylist(UE4::UObject* InFortGameStateAthena, const std::string& FortPlaylistAthenaFullName, bool ShouldSkipWarmup, bool ShouldSkipAircraft) 
		{
			auto FortPlaylistAthena = GlobalObjects->FindObjectByFullName(FortPlaylistAthenaFullName);

			if (!FortPlaylistAthena)
			{
				std::cout << "Invalid playlist name!" << std::endl;
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

		static void SwitchLevel(UE4::UObject* InController, UE4::FString InURL)
		{
			UE4::FString URL = InURL;
			UE4::ProcessEvent(InController, Offsets::SwitchLevelFunc, &URL, 0);
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

		static bool IsTargettingWeapon(UE4::UObject* InFortWeapon)
		{
			return *reinterpret_cast<bool*>(__int64(InFortWeapon) + __int64(Offsets::bIsTargetingOffset));
		}
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
		Offsets::CheatManagerOffset = GlobalObjects->FindOffset("PlayerController", "CheatManager");
		Offsets::CurrentMovementStyleOffset = GlobalObjects->FindOffset("FortPawn", "CurrentMovementStyle");
		Offsets::CharacterMovementOffset = GlobalObjects->FindOffset("Character", "CharacterMovement");
		Offsets::bInfiniteAmmoOffset = GlobalObjects->FindOffset("FortPlayerController", "bInfiniteAmmo");
		Offsets::CurrentPlaylistInfoOffset = GlobalObjects->FindOffset("FortGameStateAthena", "CurrentPlaylistInfo");
		Offsets::BasePlaylistOffset = GlobalObjects->FindOffset("PlaylistPropertyArray", "BasePlaylist");
		Offsets::bWantsToSprintOffset = GlobalObjects->FindOffset("FortPlayerController", "bWantsToSprint");
		Offsets::bSkipWarmupOffset = GlobalObjects->FindOffset("FortPlaylistAthena", "bSkipWarmup");
		Offsets::bSkipAircraftOffset = GlobalObjects->FindOffset("FortPlaylistAthena", "bSkipAircraft");
		Offsets::McpProfileGroupOffset = GlobalObjects->FindOffset("FortPlayerController", "McpProfileGroup");
		Offsets::PlayerNameOffset = GlobalObjects->FindOffset("McpProfileGroup", "PlayerName");
		Offsets::CurrentWeaponOffset = GlobalObjects->FindOffset("FortPawn", "CurrentWeapon");
		Offsets::bIsTargetingOffset = GlobalObjects->FindOffset("FortWeapon", "bIsTargeting");

		Offsets::CheatManagerClass = GlobalObjects->FindObjectByFullName("Class /Script/Engine.CheatManager");
		
		Offsets::Default__FortKismetLibrary = GlobalObjects->FindObjectByFullName("FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary");
		Offsets::Default__FortItemAndVariantSwapHelpers = GlobalObjects->FindObjectByFullName("FortItemAndVariantSwapHelpers /Script/FortniteGame.Default__FortItemAndVariantSwapHelpers");
		
		Offsets::SetTimeOfDayManagerGameplayOverrideFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortGameModeAthena.SetTimeOfDayManagerGameplayOverride");
		Offsets::BugItGoFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.BugItGo");
		Offsets::GetActorLocationFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Actor.K2_GetActorLocation");
		Offsets::JumpFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Character.Jump");
		Offsets::JumpToSafeZonePhaseFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortGameModeAthena.JumpToSafeZonePhase");
		Offsets::OnRep_CurrentPlaylistInfoFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortGameStateAthena.OnRep_CurrentPlaylistInfo");
		Offsets::PawnPlayerStateOffset = GlobalObjects->FindOffset("Controller", "PlayerState");
		Offsets::TeleportToSkyDiveFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerPawnAthena.TeleportToSkyDive");
		Offsets::EquipWeaponDefinitionFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPawn.EquipWeaponDefinition");
		Offsets::PushCosmeticOverrideOntoPawnFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortItemAndVariantSwapHelpers.PushCosmeticOverrideOntoPawn");
		Offsets::SetCanBeDamagedFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortKismetLibrary.SetCanBeDamaged");
		Offsets::DestroyAllFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.DestroyAll");
		Offsets::StartMatchFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.GameMode.StartMatch");
		Offsets::ServerReadyToStartMatchFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerController.ServerReadyToStartMatch");
		Offsets::PossessFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Controller.Possess");
		Offsets::PlayButtonFunc = GlobalObjects->FindObjectByFullName("Function /Game/Athena/UI/Matchmaking/AthenaLobbyMatchmakingPlay.AthenaLobbyMatchmakingPlay_C.BndEvt__PlayButton_K2Node_ComponentBoundEvent_0_CommonButtonClicked__DelegateSignature");
		Offsets::ReadyToStartMatchFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.GameMode.ReadyToStartMatch");
		Offsets::SummonFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.Summon");
		Offsets::SwitchLevelFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.PlayerController.SwitchLevel");
		Offsets::IsSkydivingFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerPawn.IsSkydiving");
		Offsets::IsParachuteOpenFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerPawn.IsParachuteOpen");
		Offsets::IsParachuteForcedOpenFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerPawn.IsParachuteForcedOpen");
		Offsets::SetMovementModeFunc = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CharacterMovementComponent.SetMovementMode");
		Offsets::OnRep_IsParachuteOpenFunc = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerPawn.OnRep_IsParachuteOpen");
	}

	bool bIsInGame = false;

	static void LoadAthena()
	{
		using namespace UE4;
		Controller = (UObject*)(UE4::GetFirstPlayerController(*GWorld));

		std::cout << "CurrentMovementStyle -> " << Offsets::CurrentMovementStyleOffset << std::endl;
		std::cout << "CharacterMovementOffset -> " << Offsets::CharacterMovementOffset << std::endl;
		std::cout << "IsSkydivingFunc -> " << Offsets::IsSkydivingFunc << std::endl;
		std::cout << "IsParachuteOpenFunc -> " << Offsets::IsParachuteOpenFunc << std::endl;
		std::cout << "SetMovementModeFunc -> " << Offsets::SetMovementModeFunc << std::endl;

		CheatManager = RiftAutomationUtils::CreateCheatManager(Controller);
		RiftAutomationUtils::BugItGo(CheatManager, 0, 0, 0, 0, 0, 0);
		RiftAutomationUtils::Summon(CheatManager, L"PlayerPawn_Athena_C");
		std::cout << "Summoned" << std::endl;

		Pawn = GlobalObjects->FindObject("PlayerPawn_Athena_C_");
		GameState = GlobalObjects->FindObject("Athena_GameState_C_");
		GameMode = GlobalObjects->FindObject("Athena_GameMode_C_");
		PlayerState = RiftAutomationUtils::GetPlayerState(Controller);
		CharacterMovement = RiftAutomationUtils::GetCharacterMovementComponent(Pawn);

		std::cout << "Pawn: " << Pawn << std::endl;
		std::cout << "GameState: " << GameState << std::endl;
		std::cout << "GameMode: " << GameMode << std::endl;
		std::cout << "PlayerState: " << PlayerState << std::endl;
		std::cout << "CharacterMovement: " << CharacterMovement << std::endl;

		//UE4::UObject* NewTODM = UE4::StaticLoadObject(GlobalObjects->FindObjectByFullName("Class /Script/Engine.BlueprintGeneratedClass"), nullptr, L"/Game/TimeOfDay/TODM/STW/Children/TODM_STW_Winter_2018.TODM_STW_Winter_2018_C");
		//RiftAutomationUtils::SetTimeOfDayManagerGameplayOverride(GameMode, NewTODM);

		RiftAutomationUtils::SetPlaylist(GameState, "FortPlaylistAthena /Game/Athena/Playlists/Showdown/Playlist_ShowdownAlt_Solo.Playlist_ShowdownAlt_Solo", true, true);
		std::cout << "SetPlaylist" << std::endl;

		RiftAutomationUtils::Possess(Controller, Pawn);
		std::cout << "Possessed" << std::endl;

		RiftAutomationUtils::JumpToSafeZonePhase(GameMode);
		std::cout << "JumpToSafeZonePhase" << std::endl;

		RiftAutomationUtils::OverridePawnCosmetic(Pawn, "AthenaCharacterItemDefinition /Game/Heroes/Commando/CosmeticCharacterItemDefinitions/CID_Commando_016_F_V1.CID_Commando_016_F_V1");
		RiftAutomationUtils::OverridePawnCosmetic(Pawn, "AthenaGliderItemDefinition /Game/Athena/Items/Cosmetics/Gliders/Glider_ID_260_ShapeshifterFemale.Glider_ID_260_ShapeshifterFemale");
		std::cout << "EnableSkin" << std::endl;

		RiftAutomationUtils::TeleportToSkyDive(Pawn, 100000);
		std::cout << "TeleportToSkyDive" << std::endl;

		RiftAutomationUtils::ServerReadyToStartMatch(Controller);
		std::cout << "ServerReadyToStartMatch" << std::endl;

		RiftAutomationUtils::StartMatch(GameMode);
		std::cout << "StartMatch" << std::endl;

		RiftAutomationUtils::DestroyAll(CheatManager, "FortHLODSMActor");
		std::cout << "DestroyAll" << std::endl;

		RiftAutomationUtils::SetActorGodMode(Pawn, true);
		std::cout << "GiveActorGodMode" << std::endl;

		std::cout << "Player Name: " << RiftAutomationUtils::GetAccountName() << std::endl;

		RiftAutomationUtils::SetInfiniteAmmo(Controller);
		std::cout << "InfiniteAmmo" << std::endl;

		RiftAutomationUtils::EquipWeapon(Pawn, "FortWeaponMeleeItemDefinition /Mantis/Items/UncleBrolly/WID_UncleBrolly_VR.WID_UncleBrolly_VR");
		std::cout << "EquipWeapon" << std::endl;

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
			std::cout << "Play button was clicked! " << Object->GetFullName() << std::endl;
			RiftAutomationUtils::SwitchLevel((UE4::UObject*)(UE4::GetFirstPlayerController(*GWorld)), L"Apollo_Terrain");
		}

		else if (Function == Offsets::ReadyToStartMatchFunc)
		{
			std::string ObjName = Object->GetName();
			if (ObjName.find("Athena_GameMode_C_") != std::string::npos)
			{
				std::cout << "Setting things up! " << Object->GetFullName() << std::endl;
				LoadAthena();
			}
		}

		else if (bIsInGame)
		{
			if (Object == Pawn)
			{
				std::string FuncName = Function->GetName();
				if (FuncName.find("Tick") != std::string::npos)
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

							std::cout << "IsSkydiving -> " << RiftAutomationUtils::IsSkydiving(Pawn) << std::endl;
							std::cout << "IsParachuteOpen -> " << RiftAutomationUtils::IsParachuteOpen(Pawn) << std::endl;
							std::cout << "IsParachuteForcedOpen -> " << RiftAutomationUtils::IsParachuteForcedOpen(Pawn) << std::endl;

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
						RiftAutomationUtils::SwitchLevel((UE4::UObject*)(UE4::GetFirstPlayerController(*GWorld)), L"Frontend");
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
		std::cout << "RIFT: PostInit has begun!" << std::endl;

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
				UE4::UObject* AreOffsetsAvailable = GlobalObjects->FindObject("BndEvt__PlayButton_K2Node_ComponentBoundEvent_0_CommonButtonClicked__DelegateSignature");
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
		GWorld = reinterpret_cast<UE4::UObject**>(Memory::FindPattern(FNPatterns::GWorldPattern, true, 3));
		UE4::GObjectsAddr = Memory::FindPattern(FNPatterns::UObjectArrayPattern, true, 10);
		UE4::FreeAddr = Memory::FindPattern(FNPatterns::MemoryFreePattern);
		UE4::GetObjNameAddr = Memory::FindPattern(FNPatterns::GetObjectNamePattern);
		UE4::GetFirstPlayerControllerAddr = Memory::FindPattern(FNPatterns::GetFirstPlayerControllerPattern);
		UE4::ProcessEventAddr = Memory::FindPattern(FNPatterns::ProcessEventPattern);
		UE4::StaticConstructObject_InternalAddr = Memory::FindPattern(FNPatterns::StaticConstructObject_InternalPattern);
		UE4::GetNameByIndexAddr = Memory::FindPattern(FNPatterns::GetNameByIndexPattern);
		UE4::StaticLoadObjectAddr = Memory::FindPattern(FNPatterns::StaticLoadObjectPattern);
		UE4::SpawnActorAddr = Memory::FindPattern(FNPatterns::SpawnActorPattern);
		UE4::SprintPatchAddr = Memory::FindPattern(FNPatterns::SprintPatchPattern);
		UE4::WeaponPatchAddr = Memory::FindPattern(FNPatterns::WeaponPatchPattern);

		if (!UE4::GObjectsAddr || !UE4::FreeAddr || !UE4::GetObjNameAddr || !UE4::GetFirstPlayerControllerAddr || !UE4::ProcessEventAddr || !UE4::StaticConstructObject_InternalAddr || !GWorld || !UE4::GetNameByIndexAddr || !UE4::StaticLoadObjectAddr || !UE4::SprintPatchAddr || !UE4::WeaponPatchAddr)
		{
			std::cout << "One or more patterns was incorrect." << std::endl;
			return false;
		}

		GlobalObjects = reinterpret_cast<UE4::GObjects*>(UE4::GObjectsAddr);

		CreateThread(0, 0, StartupGWorldCheckLoop, 0, 0, 0);
		return true;
	}
};