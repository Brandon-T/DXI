#include "Hooks.hpp"

std::unique_ptr<SharedMemory> SharedImageData;
const char* SharedImageName = "Local\\DXIImage_";

std::string GetProcessID()
{
    return "_" + std::to_string(getpid());
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

bool InitializeAll()
{
    return ((CreateSharedMemory(getpid()) || OpenSharedMemory(getpid())) && Initialize());
}

bool CreateSharedMemory(int ProcessID)
{
    int Width = 0, Height = 0;
    GetDesktopResolution(Width, Height);
    MessageBox(NULL, ("Mapped: " + std::string(SharedImageName) + std::to_string(ProcessID)).c_str(), "Map-ID", 0);
    SharedImageData.reset(new SharedMemory(SharedImageName + std::to_string(ProcessID)));
    return SharedImageData->MapMemory(Width || Height == 0 ? TotalImageSize : Width * Height * 4 * 2);
}

bool OpenSharedMemory(int ProcessID)
{
    int Width = 0, Height = 0;
    GetDesktopResolution(Width, Height);
    MessageBox(NULL, ("Opened: " + std::string(SharedImageName) + std::to_string(ProcessID)).c_str(), "Map-ID", 0);
    SharedImageData.reset(new SharedMemory(SharedImageName + std::to_string(ProcessID)));
    return SharedImageData->OpenMemoryMap(Width || Height == 0 ? SharedImageSize : Width * Height * 4 * 2);
}

bool UnMapSharedMemory()
{
    SharedImageData.reset(nullptr);
    return true;
}

void FlipImageBytes(void* In, void* &Out, int width, int height, unsigned int Bpp)
{
   unsigned long Chunk = (Bpp > 24 ? width * 4 : width * 3 + width % 4);
   unsigned char* Destination = static_cast<unsigned char*>(Out);
   unsigned char* Source = static_cast<unsigned char*>(In) + Chunk * (height - 1);

   while(Source != In)
   {
      std::memcpy(Destination, Source, Chunk);
      Destination += Chunk;
      Source -= Chunk;
   }
}
