#include <d3d9.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include <chrono>
#include <string>

#pragma comment(lib, "detours.lib")

typedef HMODULE (__stdcall *LoadLibrary_t)(LPCSTR dllName);
LoadLibrary_t o_LoadLibrary;

HMODULE __stdcall DXI_LoadLibrary(LPCSTR dllName)
{
	if (std::string(dllName).find("jagdx") != std::string::npos)
	{
		MessageBoxA(NULL, "Direct-X Mode", "LoadLibrary", 0);
	}
	else if (std::string(dllName).find("jaggl") != std::string::npos)
	{
		MessageBoxA(NULL, "OpenGL Mode", "LoadLibrary", 0);
	}
    return o_LoadLibrary(dllName);
}

#ifdef MS
template<typename T, typename U>
void DetourFunction(T &OriginalFunction, U HookFunction)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&reinterpret_cast<void*&>(OriginalFunction), reinterpret_cast<void*>(HookFunction));
	DetourTransactionCommit();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
}
#endif

bool __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
			o_LoadLibrary = (LoadLibrary_t) GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "LoadLibraryA");
			DetourFunction(o_LoadLibrary, DXI_LoadLibrary);
            return true;

        case DLL_PROCESS_DETACH:
            break;

        default:
            break;
    }
    return true;
}
