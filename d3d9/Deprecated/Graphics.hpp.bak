#ifndef GRAPHICS_HPP_INCLUDED
#define GRAPHICS_HPP_INCLUDED

#include <d3d9.h>
#include <D3Dx9tex.h>
#include <cstdint>
#include <memory>
#include "Images.hpp"


template<typename T>
void SafeRelease(T* &Ptr)
{
    if (Ptr)
    {
        Ptr->Release();
        Ptr = nullptr;
    }
}

class Graphics
{
    public:
        HRESULT Capture(IDirect3DDevice9* Device, const char* FilePath, int Width = 0, int Height = 0, D3DFORMAT Format = D3DFMT_UNKNOWN);
        HRESULT Capture2(IDirect3DDevice9* Device, const char* FilePath);
        HRESULT Capture3(IDirect3DDevice9* Device, std::uint8_t* &Buffer, int Width = 0, int Height = 0, D3DFORMAT Format = D3DFMT_UNKNOWN);
};

#endif // GRAPHICS_HPP_INCLUDED
