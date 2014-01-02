#include <iostream>
#include <windows.h>
#include <TlHelp32.h>

PROCESSENTRY32 GetProcessInfo(const char* ProcessName)
{
    void* hSnap = nullptr;
    PROCESSENTRY32 Proc32 = {0};

    if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
        return Proc32;

    Proc32.dwSize = sizeof(PROCESSENTRY32);
    while(Process32Next(hSnap, &Proc32))
    {
        if(_stricmp(ProcessName, Proc32.szExeFile) == 0)
        {
            CloseHandle(hSnap);
            return Proc32;
        }
    }
    CloseHandle(hSnap);
    Proc32 = {0};
    return Proc32;
}

bool IsProcessRunning(const char* pProcessName)
{
    return (GetProcessInfo(pProcessName).th32ProcessID != 0);
}

void PrintProcessInfo(const char* ProcessName)
{
    PROCESSENTRY32 Proc32 = GetProcessInfo(ProcessName);
    if (Proc32.th32ProcessID != 0)
    {
        std::cout << "  =======================================================\n";
        std::cout << "  Process Name:            " << Proc32.szExeFile           << "\n";
        std::cout << "  =======================================================\n\n";
        std::cout << "  Process ID:              " << Proc32.th32ProcessID       << "\n";
        std::cout << "  Thread Count:            " << Proc32.cntThreads          << "\n";
        std::cout << "  Priority Base:           " << Proc32.pcPriClassBase      << "\n";
        std::cout << "  Parent Process ID:       " << Proc32.th32ParentProcessID << "\n\n";
        std::cout << "  =======================================================\n";
    }
}

bool Inject(std::string Process, std::string File)
{
    if (IsProcessRunning(Process.c_str()))
    {
        HMODULE hKernel32 = nullptr;
        void* RemoteAddress = nullptr;
        char FilePath[MAX_PATH + 1] = {0};
        HANDLE ProcessHandle, hThread = nullptr;
        LPTHREAD_START_ROUTINE LoadLibraryHandle = nullptr;

        PrintProcessInfo(Process.c_str());
        PROCESSENTRY32 ProcessInfo = GetProcessInfo(Process.c_str());
        _snprintf(FilePath, MAX_PATH, File.c_str());

        if ((ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessInfo.th32ProcessID)))
        {
            if ((hKernel32 = GetModuleHandle("Kernel32.dll")))
            {
                LoadLibraryHandle = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(hKernel32, "LoadLibraryA"));
                RemoteAddress = VirtualAllocEx(ProcessHandle, nullptr, File.size() * sizeof(TCHAR), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                WriteProcessMemory(ProcessHandle, RemoteAddress, FilePath, File.size() * sizeof(TCHAR), nullptr);
                hThread = CreateRemoteThread(ProcessHandle, nullptr, 0, LoadLibraryHandle, RemoteAddress, 0, nullptr);
                WaitForSingleObject(hThread, INFINITE);
                VirtualFreeEx(ProcessHandle, RemoteAddress, File.size() * sizeof(TCHAR), MEM_RELEASE);
                CloseHandle(ProcessHandle);
                CloseHandle(hThread);
                return true;
            }
            CloseHandle(ProcessHandle);
        }
    }
    return false;
}

int main()
{
    Inject("JagexLauncher.exe", "C:/Users/Brandon/Desktop/LHook.dll");
    std::cin.get();
}
