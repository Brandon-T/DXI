#ifndef EXPORTS_HPP_INCLUDED
#define EXPORTS_HPP_INCLUDED

#include <d3d9.h>
#include <stdexcept>
#include "Library.hpp"
#include "IDirect3D9Proxy.hpp"

struct d3d9_dll
{
    HMODULE dll;
    FARPROC D3DPERF_BeginEvent;
    FARPROC D3DPERF_EndEvent;
    FARPROC D3DPERF_GetStatus;
    FARPROC D3DPERF_QueryRepeatFrame;
    FARPROC D3DPERF_SetMarker;
    FARPROC D3DPERF_SetOptions;
    FARPROC D3DPERF_SetRegion;
    FARPROC DebugSetLevel;
    FARPROC DebugSetMute;
    FARPROC Direct3D9EnableMaximizedWindowedModeShim;
    FARPROC Direct3DCreate9;
    FARPROC Direct3DCreate9Ex;
    FARPROC Direct3DShaderValidatorCreate9;
    FARPROC PSGPError;
    FARPROC PSGPSampleTexture;
};

extern d3d9_dll d3d9;
extern Library* OriginalDX;
extern IDirect3D9Proxy* D3D9Proxy;
extern "C" bool __stdcall Initialize(void);
extern "C" bool __stdcall DeInitialize(void);

#endif // EXPORTS_HPP_INCLUDED
