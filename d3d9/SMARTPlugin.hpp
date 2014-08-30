/**  © 2013, Brandon T. All Rights Reserved.
  *
  *  This file is part of the DXI Library.
  *  DXI is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  DXI is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with DXI.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef _SMART_PLUGIN
#define _SMART_PLUGIN

#include <d3d9.h>
#include <vector>
#include <cstdint>
#include <cmath>

typedef void (*_SMARTGetMousePos)(int &x, int &y);
typedef void (*_SMARTSetCapture)(bool enabled);
typedef void (*_SMARTButtonPressed)(int id, bool state);

typedef struct
{
    int version;
    void *img, *dbg;
    int width, height;
    _SMARTGetMousePos getMousePos;
    _SMARTSetCapture setCapture;
} SMARTInfo;

template<typename T>
void SafeRelease(T* &Ptr)
{
    if (Ptr)
    {
        Ptr->Release();
        Ptr = nullptr;
    }
}

typedef void (*_SMARTPluginInit)(SMARTInfo* ptr, bool* ReplaceButtons, int* ButtonCount, char*** ButtonText, int** ButtonIDs, _SMARTButtonPressed* ButtonCallback);

extern IDirect3DTexture9* Texture;
extern std::uint8_t* TexturePixels;

extern SMARTInfo* SmartGlobal;
extern bool SmartDebugEnabled;
extern bool SmartDirectXEnabled;

void BltSmartBuffer(IDirect3DDevice9* Device);
HRESULT dxReadPixels(IDirect3DDevice9* Device, void* Buffer, bool& Minimised, int &Width, int &Height, D3DFORMAT Format = D3DFMT_UNKNOWN);
void DrawCircle(IDirect3DDevice9* Device, float mx, float my, float r, D3DCOLOR colour = D3DCOLOR_RGBA(0xFF, 0x00, 0x00, 0xFF));

#endif
