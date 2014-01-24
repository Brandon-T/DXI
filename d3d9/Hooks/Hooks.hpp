#ifndef HOOKS_HPP_INCLUDED
#define HOOKS_HPP_INCLUDED

#include <cstring>
#include <vector>
#include <memory>
#include "Exports.hpp"
#include "../SharedMemory.hpp"
#include "../SMARTPlugin.hpp"

#define SharedImageSize     8294400     //Highest Resolution Support: 1920 x 1080 x sizeof(RGBA)
#define TotalImageSize      (SharedImageSize * 2)  //Image + DebugImage

extern const char* SharedImageName;
extern std::unique_ptr<SharedMemory> SharedImageData;

extern void GetDesktopResolution(int &width, int &height);

extern bool InitializeAll();

extern bool CreateSharedMemory(int ProcessId);

extern bool OpenSharedMemory(int ProcessId);

extern bool UnMapSharedMemory();

extern void FlipImageBytes(void* In, void* &Out, int width, int height, unsigned int Bpp);

#endif // HOOKS_HPP_INCLUDED
