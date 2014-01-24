#include "Graphics.hpp"

HRESULT Graphics::Capture(IDirect3DDevice9* Device, const char* FilePath, int Width, int Height, D3DFORMAT Format)
{
    IDirect3DSurface9* RenderTarget = nullptr;
    IDirect3DSurface9* DestTarget = nullptr;
    HRESULT result = Device->GetRenderTarget(0, &RenderTarget);

    if (result == S_OK)
    {
        if (Width == 0 || Height == 0 || Format == D3DFMT_UNKNOWN)
        {
            D3DSURFACE_DESC descriptor = {};
            RenderTarget->GetDesc(&descriptor);
            Width = descriptor.Width;
            Height = descriptor.Height;
            Format = descriptor.Format;
        }

        result = Device->CreateOffscreenPlainSurface(Width, Height, Format, D3DPOOL_SYSTEMMEM, &DestTarget, nullptr);
        result = Device->GetRenderTargetData(RenderTarget, DestTarget);
        result = D3DXSaveSurfaceToFile(FilePath, D3DXIFF_PNG, DestTarget, nullptr, nullptr);
    }

    SafeRelease(RenderTarget);
    SafeRelease(DestTarget);
    return result;
}

HRESULT Graphics::Capture2(IDirect3DDevice9* Device, const char* FilePath)
{
    IDirect3DSurface9* RenderTarget = nullptr;
    HRESULT result = Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &RenderTarget);
    result = D3DXSaveSurfaceToFile(FilePath, D3DXIFF_PNG, RenderTarget, nullptr, nullptr);
    SafeRelease(RenderTarget);
    return result;
}

HRESULT Graphics::Capture3(IDirect3DDevice9* Device, std::uint8_t* &Buffer, int Width, int Height, D3DFORMAT Format)
{
    IDirect3DSurface9* RenderTarget = nullptr;
    IDirect3DSurface9* DestTarget = nullptr;
    HRESULT result = Device->GetRenderTarget(0, &RenderTarget);

    if (result == S_OK)
    {
        if (Width == 0 || Height == 0 || Format == D3DFMT_UNKNOWN)
        {
            D3DSURFACE_DESC descriptor = {};
            RenderTarget->GetDesc(&descriptor);
            Width = descriptor.Width;
            Height = descriptor.Height;
            Format = descriptor.Format;
        }

        result = Device->CreateOffscreenPlainSurface(Width, Height, Format, D3DPOOL_SYSTEMMEM, &DestTarget, nullptr);
        result = Device->GetRenderTargetData(RenderTarget, DestTarget);

        D3DLOCKED_RECT rect;
        DestTarget->LockRect(&rect, 0, D3DLOCK_READONLY);

        //std::unique_ptr<std::uint8_t[]> mem(new std::uint8_t[Width * Height * 4]);
        //memcpy(mem.get(), rect.pBits, Width * Height * 4);
        Buffer = reinterpret_cast<std::uint8_t*>(rect.pBits);
    }

    SafeRelease(RenderTarget);
    SafeRelease(DestTarget);
    return result;
}
