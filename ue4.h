#pragma once
#include <Windows.h>
#include <iostream>
#include "util.h"
#include "FortUpdater/FortUpdater.h"

namespace UE4
{
    uintptr_t GObjectsAddr;
    uintptr_t FreeAddr;
    uintptr_t GetObjNameAddr;
    uintptr_t GetFirstPlayerControllerAddr;
    uintptr_t ProcessEventAddr;
    uintptr_t StaticConstructObject_InternalAddr;
    uintptr_t GetNameByIndexAddr;
    uintptr_t SpawnActorAddr;
    uintptr_t StaticLoadObjectAddr;
    uintptr_t SprintPatchAddr;
    uintptr_t WeaponPatchAddr;
    uintptr_t AthenaGamemodeCrashAddr;
    uintptr_t PlayEmoteItemInternalAddr;
    uintptr_t CrouchAddr;

    template<class T>
    struct TArray
    {
        friend struct FString;

    public:
        inline TArray()
        {
            Data = nullptr;
            Count = Max = 0;
        };

        inline int Num() const
        {
            return Count;
        };

        inline T& operator[](int i)
        {
            return Data[i];
        };

        inline const T& operator[](int i) const
        {
            return Data[i];
        };

        inline bool IsValidIndex(int i) const
        {
            return i < Num();
        }

    private:
        T* Data;
        int32_t Count;
        int32_t Max;
    };

    struct FString : private TArray<wchar_t>
    {
        inline FString()
        {
        };

        inline FString(const wchar_t* other)
        {
    #pragma warning( push )
    #pragma warning( disable : 4267 )
            Max = Count = *other ? std::wcslen(other) + 1 : 0;
    #pragma warning( pop )
            if (Count)
            {
                Data = const_cast<wchar_t*>(other);
            }
        };

        inline bool IsValid() const
        {
            return Data != nullptr;
        }

        inline const wchar_t* c_str() const
        {
            return Data;
        }

        std::string ToString() const
        {

            auto length = std::wcslen(Data);

            std::string str(length, '\0');

            std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

            return str;
        }
    };

    struct FName
    {
        int32_t ComparisonIndex;
        int32_t Number;
    };

    static void FixName(char* cName)
    {
        for (int i = 0; cName[i] != '\0'; i++)
        {
            if (cName[i] == '_')
            {
                if (cName[i + 1] == '0' ||
                    cName[i + 1] == '1' ||
                    cName[i + 1] == '2' ||
                    cName[i + 1] == '3' ||
                    cName[i + 1] == '4' ||
                    cName[i + 1] == '5' ||
                    cName[i + 1] == '6' ||
                    cName[i + 1] == '7' ||
                    cName[i + 1] == '8' ||
                    cName[i + 1] == '9')
                    cName[i] = '\0';
                break;
            }
        }
        return;
    }

    class UObject
    {
    public:
        void** Vtable;
        int32_t ObjectFlags;
        int32_t InternalIndex;
        class UObject* Class;
        FName  Name;
        class UObject* Outer;

        inline char* GetName()
        {
            FString Buffer;
            auto GetObjName = reinterpret_cast<FString * (__fastcall*)(FString*, UObject*)>(GetObjNameAddr);
            GetObjName((FString*)&Buffer, this);
            if (!Buffer.c_str())
                return (char*)("");

            char Result[1024];
            wcstombs(Result, Buffer.c_str(), sizeof(Result));
            auto FFree = reinterpret_cast<__int64(__fastcall*)(__int64)>(FreeAddr);
            FFree((uintptr_t)Buffer.c_str());
            FixName(Result);

            return Result;
        }

        inline char* GetFullName()
        {
            std::string name;
            if (this->Class != nullptr)
            {
                std::string temp;
                for (UObject* p = this->Outer; p; p = p->Outer)
                {
                    temp = std::string(p->GetName()) + "." + temp;
                }
                name = this->Class->GetName();
                name += " ";
                name += temp;
                name += this->GetName();
            }
            char* Result = (char*)name.c_str();
            return Result;
        }
    };

    class FUObjectItem {
    public:
        UObject* Object;
        DWORD Flags;
        DWORD ClusterIndex;
        DWORD SerialNumber;
        DWORD SerialNumber2;
    };

    class TUObjectArray {
    public:
        FUObjectItem* Objects[9];
    };

    class GObjects {
    public:
        inline void NumChunks(int* start, int* end) const
        {
            int cStart = 0, cEnd = 0;

            if (!cEnd)
            {
                while (1)
                {
                    if (ObjectArray->Objects[cStart] == 0)
                    {
                        cStart++;
                    }
                    else
                    {
                        break;
                    }
                }

                cEnd = cStart;
                while (1)
                {
                    if (ObjectArray->Objects[cEnd] == 0)
                    {
                        break;
                    }
                    else
                    {
                        cEnd++;
                    }
                }
            }

            *start = cStart;
            *end = cEnd;
        }

        inline UObject* GetByIndex(int32_t index) const
        {
            int cStart = 0, cEnd = 0;
            int chunkIndex = 0, chunkSize = 0xFFFF, chunkPos;
            FUObjectItem* Object;

            NumChunks(&cStart, &cEnd);

            chunkIndex = index / chunkSize;
            if (chunkSize * chunkIndex != 0 &&
                chunkSize * chunkIndex == index)
            {
                chunkIndex--;
            }

            chunkPos = cStart + chunkIndex;
            if (chunkPos < cEnd)
            {
    #pragma warning( push )
    #pragma warning( disable : 26451 )
                Object = ObjectArray->Objects[chunkPos] + (index - chunkSize * chunkIndex);
    #pragma warning( pop )
                if (!Object) { return NULL; }

                return Object->Object;
            }

            return nullptr;
        }

        inline FUObjectItem* GetItemByIndex(int32_t index) const
        {
            int cStart = 0, cEnd = 0;
            int chunkIndex = 0, chunkSize = 0xFFFF, chunkPos;
            FUObjectItem* Object;

            NumChunks(&cStart, &cEnd);

            chunkIndex = index / chunkSize;
            if (chunkSize * chunkIndex != 0 &&
                chunkSize * chunkIndex == index)
            {
                chunkIndex--;
            }

            chunkPos = cStart + chunkIndex;
            if (chunkPos < cEnd)
            {
    #pragma warning( push )
    #pragma warning( disable : 26451 )
                Object = ObjectArray->Objects[chunkPos] + (index - chunkSize * chunkIndex);
    #pragma warning( pop )
                if (!Object) { return NULL; }

                return Object;
            }

            return nullptr;
        }

        inline int32_t Num() const
        {
            return ObjectCount;
        }

        inline UObject* FindObject(const char* name)
        {
            for (int i = 0; i < this->Num(); ++i)
            {
                UObject* object = this->GetByIndex(i);
                if (object == nullptr)
                {
                    continue;
                }
                std::string objectName = object->GetName();
                if (objectName.find(name) != std::string::npos)
                {
                    return object;
                }
            }
            return nullptr;
        }

        inline UObject* FindObjectByFullName(const char* name)
        {
            for (int i = 0; i < this->Num(); ++i)
            {
                UObject* object = this->GetByIndex(i);
                if (object == nullptr)
                {
                    continue;
                }
                std::string objectName = object->GetFullName();
                if (objectName.find(name) != std::string::npos)
                {
                    return object;
                }
            }
            return nullptr;
        }

        inline UObject* FindObjectByFullNameInsensitive(const char* name)
        {
            for (int i = 0; i < this->Num(); ++i)
            {
                UObject* object = this->GetByIndex(i);
                if (object == nullptr)
                {
                    continue;
                }
                std::string objectName = Util::upper_string(object->GetFullName());
                std::string stringToFind = Util::upper_string(name);
                if (objectName.find(stringToFind) != std::string::npos)
                {
                    return object;
                }
            }
            return nullptr;
        }

        inline DWORD FindOffset(const char* Class, const char* var)
        {
            FortUpdater* Updater = new FortUpdater();
            if (Updater->Init(GObjectsAddr, GetObjNameAddr, GetNameByIndexAddr, FreeAddr))
            {
                return Updater->FindOffset(Class, var);
            }
            DEBUG_LOG("Failed to call FindOffset, one or more addresses are most likely wrong!\n");
            return NULL;
        }

    private:
        TUObjectArray* ObjectArray;
        BYTE _padding_0[0xC];
        DWORD ObjectCount;
    };


    struct FVector
    {
        float X;
        float Y;
        float Z;
    };

    struct FRotator
    {
        float Pitch;
        float Roll;
        float Yaw;
    };

    struct FGuid
    {
        int A;
        int B;
        int C;
        int D;
    };

    struct alignas(16) FQuat
    {
        float X;
        float Y;
        float Z;
        float W;
    };

    struct alignas(16) FTransform
    {
        FQuat Rotation;
        FVector Translation;
        char unknown1[0x4];
        FVector Scale3D;
        char unknown2[0x4];
    };

    struct FLinearColor
    {
        float                                              R;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
        float                                              G;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
        float                                              B;                                                        // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
        float                                              A;                                                        // 0x000C(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

        inline FLinearColor()
            : R(0), G(0), B(0), A(0)
        { }

        inline FLinearColor(float r, float g, float b, float a)
            : R(r),
            G(g),
            B(b),
            A(a)
        { }
    };

    static void* GetFirstPlayerController(void* World)
    {
        auto fGetFirstPlayerController = reinterpret_cast<void* (__fastcall*)(void*)>(GetFirstPlayerControllerAddr);
        return fGetFirstPlayerController(World);
    }

    static UObject* StaticConstructObject_Internal(
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
        auto fStaticConstructObject_Internal = reinterpret_cast<UObject* (__fastcall*)(void*, void*, void*, int, unsigned int, void*, bool, void*, bool)>(StaticConstructObject_InternalAddr);
        return fStaticConstructObject_Internal(Class, InOuter, Name, SetFlags, InternalSetFlags, Template, bCopyTransientsFromClassDefaults, InstanceGraph, bAssumeTemplateIsArchetype);
    }

    static void* ProcessEvent(void* obj, void* func, void* param, void* ret)
    {
        auto fProcessEvent = reinterpret_cast<void* (__fastcall*)(void*, void*, void*, void*)>(ProcessEventAddr);
        return fProcessEvent(obj, func, param, ret);
    }

    static UE4::UObject* StaticLoadObject(UObject* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* FileName = nullptr, uint32_t LoadFlags = 0, void* Sandbox = nullptr, bool bAllowObjectReconciliation = false, void* InstancingContext = nullptr)
    {
        auto fStaticLoadObject = reinterpret_cast<UE4::UObject* (__fastcall*)(UObject*, UObject*, const TCHAR*, const TCHAR*, uint32_t, void*, bool, void*)>(StaticLoadObjectAddr);
        return fStaticLoadObject(Class, InOuter, Name, FileName, LoadFlags, Sandbox, bAllowObjectReconciliation, InstancingContext);
    }

    static UObject* SpawnActor(UObject* World, UObject* Class, FVector const& Location, FRotator const& Rotation)
    {
        auto fSpawnActor = reinterpret_cast<UObject* (__fastcall*)(UObject*, UObject*, FVector const&, FRotator const&, void*)>(SpawnActorAddr);
        char SpawnParameters[0x4C]; //0x4C
        return fSpawnActor(World, Class, Location, Rotation, SpawnParameters);
    }
}