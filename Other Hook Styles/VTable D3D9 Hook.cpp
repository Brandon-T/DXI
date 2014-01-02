#include <windows.h>
#include <d3d9.h>
#include <memory>
#include <thread>

typedef HRESULT (__stdcall *EndScene_t)(IDirect3DSurface9* Surface);
EndScene_t o_EndScene;

HRESULT __stdcall DXI_EndScene(IDirect3DSurface9* Surface)
{
    MessageBox(NULL, "EndScene Hook", "DXI_EndScene", 0);
    return o_EndScene(Surface);
}

bool InitialiseHooks();

bool __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            std::thread([&] {InitialiseHooks();}).detach();
            return true;

        case DLL_PROCESS_DETACH:
            break;

        default:
            break;
    }
    return true;
}

DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, const char* szMask)
{
    auto bCompare = [](const BYTE* pData, const BYTE* bMask, const char* szMask) -> bool
    {
        for(; *szMask; ++szMask, ++pData, ++bMask)
            if(*szMask == 'x' && *pData != *bMask)
                return 0;
        return (*szMask) == 0;
    };

    for(DWORD i = 0; i < dwLen; ++i)
        if (bCompare(reinterpret_cast<std::uint8_t*>(dwAddress + i), bMask, szMask))
            return static_cast<DWORD>(dwAddress + i);
    return 0;
}

void* DetourFunction(std::uint8_t* OrigFunc, std::uint8_t* HookFunc, int JumpLength)
{
    DWORD dwProt = 0;
    std::uint8_t* jmp = new std::uint8_t[JumpLength + 5];
    VirtualProtect(OrigFunc, JumpLength, PAGE_READWRITE, &dwProt);
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

DWORD WINAPI VTableRepatch()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        DetourFunction(reinterpret_cast<std::uint8_t*>(o_EndScene), reinterpret_cast<std::uint8_t*>(&DXI_EndScene), 5);
    }
    return 0;
}

bool InitialiseHooks()
{
    while(!GetModuleHandle("d3d9.dll"))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    DWORD* VTable = nullptr;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    static std::uint8_t bMask[] = {0xC7, 0x06, 0x00, 0x00, 0x00, 0x00, 0x89, 0x86, 0x00, 0x00, 0x00, 0x00, 0x89, 0x86};
    DWORD Address = FindPattern(reinterpret_cast<DWORD>(GetModuleHandle("d3d9.dll")), 0x128000, bMask, "xx????xx????xx");
    memcpy(&VTable, reinterpret_cast<void*>(Address + 2), 4);

    o_EndScene = reinterpret_cast<EndScene_t>(VTable[42]);
    DetourFunction(reinterpret_cast<std::uint8_t*>(o_EndScene), reinterpret_cast<std::uint8_t*>(&DXI_EndScene), 5);
    std::thread([&] {VTableRepatch();}).detach();
    return VTable != nullptr;
}
