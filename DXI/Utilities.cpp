#include "Utilities.hpp"

bool ExtractResource(short resourceID, const char* outputFilename, const char* resName)
{
    bool success = false;
    try
    {
        HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceID), resName);
        if (hResource == 0) {return false;}

        HGLOBAL hFileResource = LoadResource(NULL, hResource);
        if (hFileResource == 0) {return false;}

        LPVOID lpFile = LockResource(hFileResource);
        if (lpFile == 0) {return false;}

        DWORD dwSize = SizeofResource(NULL, hResource);
        if (dwSize == 0) {return false;}

        HANDLE hFile = CreateFile(outputFilename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        HANDLE hFilemap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dwSize, NULL);
        if (hFilemap == 0) {return false;}

        LPVOID lpBaseAddress = MapViewOfFile(hFilemap, FILE_MAP_WRITE, 0, 0, 0);

        CopyMemory(lpBaseAddress, lpFile, dwSize);

        UnmapViewOfFile(lpBaseAddress);
        CloseHandle(hFilemap);
        CloseHandle(hFile);

        return true;
    }
    catch (...) {}
    return success;
}

bool ExtractD3D9X_43(const char* PluginsPath)
{
    if (PluginsPath != nullptr && strlen(PluginsPath))
    {
        char buffer[256] = {0};
        strcat(buffer, PluginsPath);
        strcat(buffer, "/D3DX9_43.dll");
        return ExtractResource(IDR_D3D9X43, buffer, "BINARY");
    }
    return ExtractResource(IDR_D3D9X43, "D3DX9_43.dll", "BINARY");
}

bool FindD3DX9_43(const char* PluginsPath)
{
    HMODULE module = nullptr;

    if (PluginsPath != nullptr && strlen(PluginsPath))
    {
        char buffer[512] = {0};
        strcat(buffer, PluginsPath);
        strcat(buffer, "/D3DX9_43.dll");
        module = LoadLibrary(buffer);
    }
    else
    {
        char buffer[512] = {0};
        int len = GetCurrentDirectory(sizeof(buffer) / sizeof(buffer[0]), buffer);
        std::replace(&buffer[0], &buffer[len], '\\', '/');
        strcat(buffer, "/D3DX9_43.dll");
        module = LoadLibrary(buffer);
    }

    if (!module)
    {
        #ifdef __x86_64
        module = LoadLibrary("System32/D3DX9_43.dll");
        #else
        module = LoadLibrary("SysWOW64/D3DX9_43.dll");
        #endif
    }

    if (module)
    {
        FreeLibrary(module);
        return true;
    }

    return false;
}
