#pragma once
#include <Windows.h>
#include <iostream>
#include "offsets.h"
#include "util.h"

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

class FName
{
public:
    int32_t nComparisonIndex;
    int32_t nNumber;
};

struct FVector
{
    float X;
    float Y;
    float Z;
};

struct FCustomCharacterData
{
    unsigned char                                      WasPartReplicatedFlags;                                   // 0x0000(0x0001) (ZeroConstructor, IsPlainOldData)
    unsigned char                                      UnknownData00[0x3];                                       // 0x0001(0x0003) MISSED OFFSET
    int                                                RequiredVariantPartFlags;                                 // 0x0004(0x0004) (ZeroConstructor, IsPlainOldData)
    void* Parts[0x6];                                               // 0x0008(0x0008) (ZeroConstructor, IsPlainOldData)
    void* Charms[0x4];                                              // 0x0038(0x0008) (ZeroConstructor, IsPlainOldData)
    TArray<void*>                VariantRequiredCharacterParts;                            // 0x0058(0x0010) (ZeroConstructor)
    bool                                               bReplicationFailed;                                       // 0x0068(0x0001) (ZeroConstructor, Transient, IsPlainOldData, RepSkip, RepNotify, Interp, NonTransactional, EditorOnly, NoDestructor, AutoWeak, ContainsInstancedReference, AssetRegistrySearchable, SimpleDisplay, AdvancedDisplay, Protected, BlueprintCallable, BlueprintAuthorityOnly, TextExportTransient, NonPIEDuplicateTransient, ExposeOnSpawn, PersistentInstance, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic, NativeAccessSpecifierProtected, NativeAccessSpecifierPrivate)
    unsigned char                                      UnknownData01[0x7];                                       // 0x0069(0x0007) MISSED OFFSET
};

struct FPlaylistPropertyArray
{
    unsigned char UnknownData00[0x120];
    void* BasePlaylist;
};

class AFortGameStateAthena
{
public:
    unsigned char UnknownData00[0x1BC0];
    FPlaylistPropertyArray CurrentPlaylistInfo;
};

class AFortPlayerState
{
public:
    unsigned char UnknownData00[0x4D0];
    FCustomCharacterData CharacterData;
};

class TestPawn
{
public:
    unsigned char UnknownData00[0x6B0];
    uint8_t CurrentMovementStyle;
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
        auto GetObjName = reinterpret_cast<FString* (__fastcall*)(FString*, UObject*)>(util::GET_BASE_ADDRESS() + GETOBJNAME_OFFSET);
        GetObjName((FString*)&Buffer, this);
        if (!Buffer.c_str())
            return (char*)("");

        char Result[1024];
        wcstombs(Result, Buffer.c_str(), sizeof(Result));
        auto FFree = reinterpret_cast<__int64(__fastcall*)(__int64)>(util::GET_BASE_ADDRESS() + FREE_OFFSET);
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

    inline UObject* FindObject(const std::string& name)
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

    inline UObject* FindObjectByFullName(const std::string& name)
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

    inline void Test()
    {
        std::ofstream MyFile("ObjectDump.txt");

        for (int i = 0; i < this->Num(); ++i)
        {
            UObject* object = this->GetByIndex(i);
            if (object == nullptr)
            {
                continue;
            }
            std::string objectName = object->GetFullName();
            MyFile << objectName << std::endl;
        }

        MyFile.close();
    }

private:
    TUObjectArray* ObjectArray;
    BYTE _padding_0[0xC];
    DWORD ObjectCount;
};