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

extern "C" void __stdcall DXHook_D3DPERF_BeginEvent();
extern "C" void __stdcall DXHook_D3DPERF_EndEvent();
extern "C" void __stdcall DXHook_D3DPERF_GetStatus();
extern "C" void __stdcall DXHook_D3DPERF_QueryRepeatFrame();
extern "C" void __stdcall DXHook_D3DPERF_SetMarker();
extern "C" void __stdcall DXHook_D3DPERF_SetOptions();
extern "C" void __stdcall DXHook_D3DPERF_SetRegion();
extern "C" void __stdcall DXHook_DebugSetLevel();
extern "C" void __stdcall DXHook_DebugSetMute();
extern "C" IDirect3D9* __stdcall DXHook_Direct3DCreate9(UINT SDKVersion);
extern "C" void __stdcall DXHook_Direct3DCreate9Ex();
extern "C" void __stdcall DXHook_Direct3DShaderValidatorCreate9();
extern "C" void __stdcall DXHook_PSGPError();
extern "C" void __stdcall DXHook_PSGPSampleTexture();

#endif // EXPORTS_HPP_INCLUDED
