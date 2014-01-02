#include <iostream>
#include <windows.h>
#include <map>

class VHook
{
    private:
        std::uint32_t* VTable;
        std::map<int, std::uint32_t> HookFunctions;
        std::map<int, std::uint32_t> OrigFunctions;
        DWORD* GetVTableAddress(void* pObject);

    public:
        VHook(std::uint32_t* VTable);
        VHook(DWORD* VTable);

        template<typename T>
        VHook(T* VTable);

        template<typename T>
        void Patch(T* HookFunction, int Index);
        void Remove(int Index);
        void Erase(int Index);
};

VHook::VHook(DWORD* VTable) : VTable(reinterpret_cast<std::uint32_t*>(VTable)) {}

VHook::VHook(std::uint32_t* VTable) : VTable(VTable) {}

template<typename T>
VHook::VHook(T* VTable) : VTable(reinterpret_cast<std::uint32_t*>(this->GetVTableAddress(VTable))) {}

DWORD* VHook::GetVTableAddress(void* pObject)
{
    return *reinterpret_cast<DWORD**>(pObject);
}

template<typename T>
void VHook::Patch(T* HookFunction, int Index)
{
    DWORD dwOldProt = 0, *OrigFunction = nullptr;
    VirtualProtect(&this->VTable[Index], sizeof(int), PAGE_EXECUTE_READWRITE, &dwOldProt);
    OrigFunction = reinterpret_cast<DWORD*>(this->VTable[Index]);
    this->VTable[Index] = reinterpret_cast<DWORD>(HookFunction);
    VirtualProtect(&this->VTable[Index], sizeof(int), dwOldProt, &dwOldProt);
    this->OrigFunctions.insert( {Index, reinterpret_cast<std::uint32_t>(OrigFunction)});
    this->HookFunctions.insert( {Index, reinterpret_cast<std::uint32_t>(HookFunction)});
}

void VHook::Remove(int Index)
{
    DWORD dwOldProt = 0;
    VirtualProtect(&this->VTable[Index], sizeof(int), PAGE_EXECUTE_READWRITE, &dwOldProt);
    this->VTable[Index] = static_cast<DWORD>(this->OrigFunctions[Index]);
    VirtualProtect(&this->VTable[Index], sizeof(int), dwOldProt, &dwOldProt);
}

void VHook::Erase(int Index)
{
    DWORD dwOldProt = 0;
    VirtualProtect(&this->VTable[Index], sizeof(int), PAGE_EXECUTE_READWRITE, &dwOldProt);
    this->VTable[Index] = static_cast<DWORD>(this->OrigFunctions[Index]);
    VirtualProtect(&this->VTable[Index], sizeof(int), dwOldProt, &dwOldProt);
    this->OrigFunctions.erase(Index);
    this->HookFunctions.erase(Index);
}

typedef void (__stdcall *pSleep)(DWORD Milliseconds);
pSleep oSleep;

void __stdcall hSleep(DWORD Milliseconds)
{
    std::cout << "Hooked!\n";
    //oSleep(Milliseconds);
}

void* DetourFunction(std::uint8_t* OrigFunc, const std::uint8_t* HookFunc, const int JumpLength)
{
    DWORD dwProt = 0;
    std::uint8_t* jmp = new std::uint8_t[JumpLength + 5];

    VirtualProtect(OrigFunc, JumpLength, PAGE_EXECUTE_READWRITE, &dwProt);
    memcpy(jmp, OrigFunc, JumpLength);

    jmp += JumpLength;
    jmp[0] = 0xE9;

    *reinterpret_cast<DWORD*>(jmp + 1) = static_cast<DWORD>(OrigFunc + JumpLength - jmp) - 5;
    memset(OrigFunc, 0x90, JumpLength);

    OrigFunc[0] = 0xE9;
    *reinterpret_cast<DWORD*>(OrigFunc + 1) = static_cast<DWORD>(HookFunc - OrigFunc) - 5;

    VirtualProtect(OrigFunc, JumpLength, dwProt, &dwProt);
    return (jmp - JumpLength);
}

#include "Detour.hpp"

int main()
{
    oSleep = reinterpret_cast<pSleep>(GetProcAddress(GetModuleHandle("kernel32.dll"), "Sleep"));
    Detour D(oSleep, hSleep);
    D.Patch();
    Sleep(1000);
    oSleep = D.OriginalFunction<pSleep>();
    oSleep(1000);

    return 0;
}
