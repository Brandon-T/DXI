#include "Exports.hpp"

d3d9_dll d3d9;
Library* OriginalDX = nullptr;
IDirect3D9Proxy* D3D9Proxy = nullptr;

bool __stdcall Initialize(void)
{
    char Root[MAX_PATH];
    GetSystemDirectoryA(Root, MAX_PATH);
    #ifdef _MSC_VER
        strcat_s(Root, "\\d3d9.dll");
    #else
        strcat(Root, "\\d3d9.dll");
    #endif

    OriginalDX = new Library(Root);
	try
	{
	    OriginalDX->FunctionAddress(d3d9.D3DPERF_BeginEvent, "D3DPERF_BeginEvent");
	    OriginalDX->FunctionAddress(d3d9.D3DPERF_EndEvent, "D3DPERF_EndEvent");
	    OriginalDX->FunctionAddress(d3d9.D3DPERF_GetStatus, "D3DPERF_GetStatus");
	    OriginalDX->FunctionAddress(d3d9.D3DPERF_QueryRepeatFrame, "D3DPERF_QueryRepeatFrame");
	    OriginalDX->FunctionAddress(d3d9.D3DPERF_SetMarker, "D3DPERF_SetMarker");
	    OriginalDX->FunctionAddress(d3d9.D3DPERF_SetOptions, "D3DPERF_SetOptions");
	    OriginalDX->FunctionAddress(d3d9.D3DPERF_SetRegion, "D3DPERF_SetRegion");
	    OriginalDX->FunctionAddress(d3d9.DebugSetLevel, "DebugSetLevel");
	    OriginalDX->FunctionAddress(d3d9.DebugSetMute, "DebugSetMute");
	    //OriginalDX->FunctionAddress(d3d9.Direct3D9EnableMaximizedWindowedModeShim, "Direct3D9EnableMaximizedWindowedModeShim");
		OriginalDX->FunctionAddress(d3d9.Direct3DCreate9, "Direct3DCreate9");
		OriginalDX->FunctionAddress(d3d9.Direct3DCreate9Ex, "Direct3DCreate9Ex");
		OriginalDX->FunctionAddress(d3d9.Direct3DShaderValidatorCreate9, "Direct3DShaderValidatorCreate9");
		OriginalDX->FunctionAddress(d3d9.PSGPError, "PSGPError");
		OriginalDX->FunctionAddress(d3d9.PSGPSampleTexture, "PSGPSampleTexture");
	}
	catch(std::exception &e)
	{
	    MessageBox(nullptr, e.what(), "ERROR!", 0);
		return false;
	}
	return true;
}

bool __stdcall DeInitialize(void)
{
	if (OriginalDX)
	{
		delete OriginalDX;
		OriginalDX = nullptr;
		return true;
	}
	return false;
}

void __stdcall DXHook_D3DPERF_BeginEvent()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.D3DPERF_BeginEvent]}
    #else
    __asm("jmp *%0":: "r" (d3d9.D3DPERF_BeginEvent):);
    #endif
}

void __stdcall DXHook_D3DPERF_EndEvent()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.D3DPERF_EndEvent]}
    #else
    __asm("jmp *%0":: "r" (d3d9.D3DPERF_EndEvent):);
    #endif
}

void __stdcall DXHook_D3DPERF_GetStatus()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.D3DPERF_GetStatus]}
    #else
    __asm("jmp *%0":: "r" (d3d9.D3DPERF_GetStatus):);
    #endif
}

void __stdcall DXHook_D3DPERF_QueryRepeatFrame()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.D3DPERF_QueryRepeatFrame]}
    #else
    __asm("jmp *%0":: "r" (d3d9.D3DPERF_QueryRepeatFrame):);
    #endif
}

void __stdcall DXHook_D3DPERF_SetMarker()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.D3DPERF_SetMarker]}
    #else
    __asm("jmp *%0":: "r" (d3d9.D3DPERF_SetMarker):);
    #endif
}

void __stdcall DXHook_D3DPERF_SetOptions()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.D3DPERF_SetOptions]}
    #else
    __asm("jmp *%0":: "r" (d3d9.D3DPERF_SetOptions):);
    #endif
}

void __stdcall DXHook_D3DPERF_SetRegion()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.D3DPERF_SetRegion]}
    #else
    __asm("jmp *%0":: "r" (d3d9.D3DPERF_SetRegion):);
    #endif
}

void __stdcall DXHook_DebugSetLevel()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.DebugSetLevel]}
    #else
    __asm("jmp *%0":: "r" (d3d9.DebugSetLevel):);
    #endif
}

void __stdcall DXHook_DebugSetMute()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.DebugSetMute]}
    #else
    __asm("jmp *%0":: "r" (d3d9.DebugSetMute):);
    #endif
}

IDirect3D9* __stdcall DXHook_Direct3DCreate9(UINT SDKVersion)
{
    typedef IDirect3D9* (__stdcall *D3D9_Type) (UINT SDKVersion);
    IDirect3D9* pOriginal = (reinterpret_cast<D3D9_Type>(d3d9.Direct3DCreate9))(SDKVersion);
    return (D3D9Proxy = new IDirect3D9Proxy(pOriginal));
}

void __stdcall DXHook_Direct3DCreate9Ex()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.Direct3DCreate9Ex]}
    #else
    __asm("jmp *%0":: "r" (d3d9.Direct3DCreate9Ex):);
    #endif
}

void __stdcall DXHook_Direct3DShaderValidatorCreate9()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.Direct3DShaderValidatorCreate9]}
    #else
    __asm("jmp *%0":: "r" (d3d9.Direct3DShaderValidatorCreate9):);
    #endif
}

void __stdcall DXHook_PSGPError()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.PSGPError]}
    #else
    __asm("jmp *%0":: "r" (d3d9.PSGPError):);
    #endif
}

void __stdcall DXHook_PSGPSampleTexture()
{
    #ifdef _MSC_VER
    _asm{jmp[d3d9.PSGPSampleTexture]}
    #else
    __asm("jmp *%0":: "r" (d3d9.PSGPSampleTexture):);
    #endif
}
