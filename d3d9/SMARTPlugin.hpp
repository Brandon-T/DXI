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

#include <d3dx9.h>
#include <vector>
#include "Graphics.hpp"

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



typedef void (*_SMARTPluginInit)(SMARTInfo* ptr, bool* ReplaceButtons, int* ButtonCount, char*** ButtonText, int** ButtonIDs, _SMARTButtonPressed* ButtonCallback);

extern ID3DXSprite* Sprite;
extern IDirect3DTexture9* Texture;
extern unsigned char* TexturePixels;
extern SMARTInfo* SmartGlobal;
extern bool SmartDebugEnabled;
extern bool SmartDirectXEnabled;
void BltSmartBuffer(IDirect3DDevice9* Device);
void FlipImageBytes(void* In, void* &Out, int width, int height, unsigned int Bpp = 32);
HRESULT dxReadPixels(IDirect3DDevice9* Device, void* Buffer, HDC &DC, int &Width, int &Height, D3DFORMAT Format = D3DFMT_UNKNOWN);
void DrawCircle(IDirect3DDevice9* Device, float mx, float my, float r, D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0));

#endif
