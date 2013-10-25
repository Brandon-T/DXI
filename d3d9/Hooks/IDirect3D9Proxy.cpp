#include "IDirect3D9Proxy.hpp"

IDirect3D9Proxy::~IDirect3D9Proxy() {}

IDirect3D9Proxy::IDirect3D9Proxy(IDirect3D9* pOriginal)
{
    this->ptr_IDirect3D9 = pOriginal;
}

HRESULT IDirect3D9Proxy::QueryInterface(REFIID riid, void** ppvObj)
{
    HRESULT hRes = ptr_IDirect3D9->QueryInterface(riid, ppvObj);
    *ppvObj = hRes == S_OK ? this : nullptr;
    return hRes;
}

ULONG IDirect3D9Proxy::AddRef()
{
    return ptr_IDirect3D9->AddRef();
}

ULONG IDirect3D9Proxy::Release()
{
    return ptr_IDirect3D9->Release();
}

HRESULT IDirect3D9Proxy::RegisterSoftwareDevice(void* pInitializeFunction)
{
    return ptr_IDirect3D9->RegisterSoftwareDevice(pInitializeFunction);
}

UINT IDirect3D9Proxy::GetAdapterCount()
{
    return ptr_IDirect3D9->GetAdapterCount();
}

HRESULT IDirect3D9Proxy::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
{
    return ptr_IDirect3D9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT IDirect3D9Proxy::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
    return ptr_IDirect3D9->GetAdapterModeCount(Adapter, Format);
}

HRESULT IDirect3D9Proxy::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
    return ptr_IDirect3D9->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT IDirect3D9Proxy::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
    return ptr_IDirect3D9->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT IDirect3D9Proxy::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
    return ptr_IDirect3D9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT IDirect3D9Proxy::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
    return ptr_IDirect3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT IDirect3D9Proxy::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
    return ptr_IDirect3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT IDirect3D9Proxy::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
    return ptr_IDirect3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT IDirect3D9Proxy::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
    return ptr_IDirect3D9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT IDirect3D9Proxy::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
    return ptr_IDirect3D9->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR IDirect3D9Proxy::GetAdapterMonitor(UINT Adapter)
{
    return ptr_IDirect3D9->GetAdapterMonitor(Adapter);
}

HRESULT IDirect3D9Proxy::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
    IDirect3DDevice9* pDirect3DDevice9 = nullptr;
    HRESULT hRes = ptr_IDirect3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &pDirect3DDevice9);
    *ppReturnedDeviceInterface = new Direct3DDevice9Proxy(ppReturnedDeviceInterface);
    return hRes;
}
