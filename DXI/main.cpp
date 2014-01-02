#include <windows.h>
#include <memory>
#include "SharedMemory.hpp"

#define ExportCount         3
#define SharedImageSize     8294400     //Highest Resolution Support: 1920 x 1080 x sizeof(RGBA)
#define TotalImageSize      (SharedImageSize * 2)  //Image + DebugImage
#define SharedHookSize      5000000

HINSTANCE hInstance = nullptr;
std::unique_ptr<SharedMemory> SharedImageData;
std::unique_ptr<SharedMemory> SharedHookData;
std::string SharedImageName = "Local\\DXIImage_";

char* Exports[] = {
    (char*)"DXISetup", (char*)"Function DXISetup(ProcessID: Integer): Boolean;",
    (char*)"DXIImagePointer", (char*)"Function DXIImagePointer: Pointer;",
    (char*)"DXIDebugPointer", (char*)"Function DXIDebugPointer: Pointer;"
};

extern "C" int __declspec(dllexport) GetPluginABIVersion()
{
    return 2;
}

extern "C" int __declspec(dllexport) GetFunctionCount()
{
    return ExportCount;
}

extern "C" int __declspec(dllexport) GetFunctionInfo(int Index, void* &Address, char* &Definition)
{
    if (Index < ExportCount)
    {
        Address = reinterpret_cast<void*>(GetProcAddress(hInstance, Exports[Index * 2]));
		#ifdef _MSC_VER
		#pragma warning(disable: 4996)
		strcpy(Definition, Exports[Index * 2 + 1]);
        //strcpy_s(Definition, Exports[Index * 2 + 1]);
		#else
		strcpy(Definition, Exports[Index * 2 + 1]);
		#endif
        return Index;
    }
    return -1;
}

void GetDesktopResolution(int &width, int &height)
{
    #if defined _WIN32 || defined _WIN64
    RECT desktop = {0};
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    width = desktop.right;
    height = desktop.bottom;
    #endif
}

bool CreateSharedMemory(int ProcessID)
{
    int Width = 0, Height = 0;
    GetDesktopResolution(Width, Height);
    SharedImageData.reset(new SharedMemory(SharedImageName + std::to_string(ProcessID)));
    return SharedImageData->MapMemory(Width || Height == 0 ? TotalImageSize : Width * Height * 4 * 2);
}

bool OpenSharedMemory(int ProcessID)
{
    int Width = 0, Height = 0;
    GetDesktopResolution(Width, Height);
    SharedImageData.reset(new SharedMemory(SharedImageName + std::to_string(ProcessID)));
    return SharedImageData->OpenMemoryMap(Width || Height == 0 ? SharedImageSize : Width * Height * 4 * 2);
}

bool UnMapSharedMemory()
{
    SharedImageData.reset(nullptr);
    return true;
}

extern "C" bool __declspec(dllexport) DXISetup(int ProcessID)
{
    return (CreateSharedMemory(ProcessID) || OpenSharedMemory(ProcessID));
}

extern "C" void* __declspec(dllexport) DXIImagePointer()
{
    return SharedImageData ? SharedImageData->GetDataPointer() : nullptr;
}

extern "C" void* __declspec(dllexport) DXIDebugPointer()
{
    return SharedImageData ? reinterpret_cast<std::uint8_t*>(SharedImageData->GetDataPointer()) + SharedImageSize : nullptr;
}

extern "C" bool __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            hInstance = hinstDLL;
            return true;

        case DLL_PROCESS_DETACH:
            UnMapSharedMemory();
            break;

        default:
            break;
    }
    return true;
}
