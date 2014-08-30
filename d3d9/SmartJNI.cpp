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

#include "SmartPlugin.hpp"

IDirect3DTexture9* Texture = nullptr;
std::uint8_t* TexturePixels = nullptr;

SMARTInfo* SmartGlobal = nullptr;
bool SmartDebugEnabled = false;
bool SmartDirectXEnabled = true;

void SMARTButtonPressed(int ID, bool State)
{
    switch(ID)
    {
        case 100:
            if (State)
            {
                SmartGlobal->setCapture(false);
                SmartDirectXEnabled = true;
            }
            else
            {
                SmartGlobal->setCapture(true);
                SmartDirectXEnabled = false;
            }
            break;

        case 101:
            SmartDebugEnabled = State ? false : true;
            break;

    }
}

extern "C" void SMARTPluginInit(SMARTInfo* ptr, bool* ReplaceButtons, int* ButtonCount, char** * ButtonText, int** ButtonIDs, _SMARTButtonPressed* ButtonCallback)
{
    SmartGlobal = ptr;
    if (ptr)
    {
        *ReplaceButtons = true;
        char** ButtonTexts = new char* [2];
        ButtonTexts[0] = const_cast<char*>("Disable Direct-X_Enable Direct-X");
        ButtonTexts[1] = const_cast<char*>("Enable Debug_Disable dxDebug");

        int* IDs = new int[2];
        IDs[0] = 100;
        IDs[1] = 101;

        *ButtonCount = 2;
        *ButtonText = ButtonTexts;
        *ButtonIDs = IDs;
        *ButtonCallback = &SMARTButtonPressed;
    }
}

#define VERTEX_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define VERTEX_FVF_TEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct D3DVertex
{
    float X, Y, Z, RHW;
    unsigned int Colour;
    float U, V;
};

void LoadTexture(IDirect3DDevice9* Device, std::uint8_t* buffer, int width, int height, IDirect3DTexture9* &Texture)
{
    Device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &Texture, 0);

    D3DLOCKED_RECT rect;
    Texture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
    TexturePixels = static_cast<std::uint8_t*>(rect.pBits);
    Texture->UnlockRect(0);
    memcpy(TexturePixels, &buffer[0], width * height * 4);
}

void DrawTexture(IDirect3DDevice9* Device, IDirect3DTexture9* Texture, float X1, float Y1, float X2, float Y2)
{
    /**http://msdn.microsoft.com/en-us/library/windows/desktop/bb219690%28v=vs.85%29.aspx**/
    /**Offsets needed because of the above**/

    float UOffset = 0.5f / (float)(X2 - X1);
    float VOffset = 0.5f / (float)(Y2 - Y1);

    D3DVertex Vertices[] =
    {
        {X1, Y1, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 0.0f + UOffset, 0.0f + VOffset},
        {X2, Y1, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 1.0f + UOffset, 0.0f + VOffset},
        {X1, Y2, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 0.0f + UOffset, 1.0f + VOffset},
        {X2, Y2, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 1.0f + UOffset, 1.0f + VOffset}
    };

    Device->SetFVF(VERTEX_FVF_TEX);
    Device->SetTexture(0, Texture);
    Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertices, sizeof(D3DVertex));
    Device->SetTexture(0, nullptr);
}

void DrawCircle(IDirect3DDevice9* Device, float CX, float CY, float Radius, D3DCOLOR Colour)
{
    static const int Resolution = 10;
    D3DVertex Vertices[Resolution];

    for (int I = 0; I < Resolution; ++I)
    {
        Vertices[I].X = CX + Radius * std::cos(3.141592654f * (I / (Resolution / 2.0f)));
        Vertices[I].Y = CY + Radius * std::sin(3.141592654f * (I / (Resolution / 2.0f)));
        Vertices[I].Z = 0.0f;
        Vertices[I].RHW = 1.0f;
        Vertices[I].Colour = Colour;
        Vertices[I].U = 0.0f;
        Vertices[I].V = 0.0f;
    }

    Device->SetFVF(VERTEX_FVF_TEX);
    Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, Resolution - 2, Vertices, sizeof(D3DVertex));
}

void BltSmartBuffer(IDirect3DDevice9* Device)
{
    if (SmartGlobal != nullptr)
    {
        std::uint8_t* Ptr = reinterpret_cast<std::uint8_t*>(SmartGlobal->dbg);
        for (int I = 0; I < SmartGlobal->height; ++I)
        {
            for (int J = 0; J < SmartGlobal->width; ++J)
            {
                std::uint8_t B = *(Ptr++);
                std::uint8_t G = *(Ptr++);
                std::uint8_t R = *(Ptr++);
                *(Ptr++) = (B == 0 && G == 0 && R == 0) ? 0 : 0xFF;
            }
        }

        if (!Texture) /**Set Device->Reset for more info.**/
        {
            LoadTexture(Device, static_cast<unsigned char*>(SmartGlobal->dbg), SmartGlobal->width, SmartGlobal->height, Texture);
        }
        /*else
        {
            memcpy(TexturePixels, SmartGlobal->dbg, SmartGlobal->width * SmartGlobal->height * 4);
        }*/

        DrawTexture(Device, Texture, 0, 0, SmartGlobal->width, SmartGlobal->height);
        SafeRelease(Texture);
    }
}

HRESULT dxReadPixels(IDirect3DDevice9* Device, void* Buffer, bool& Minimised, int& Width, int& Height, D3DFORMAT Format)
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

        HDC DC = nullptr;
        RenderTarget->GetDC(&DC);
        Minimised = IsIconic(WindowFromDC(DC));
        RenderTarget->ReleaseDC(DC);
        result = Device->CreateOffscreenPlainSurface(Width, Height, Format, D3DPOOL_SYSTEMMEM, &DestTarget, nullptr);
        result = Device->GetRenderTargetData(RenderTarget, DestTarget);

        D3DLOCKED_RECT rect;
        DestTarget->LockRect(&rect, 0, D3DLOCK_READONLY);
        memcpy(Buffer, rect.pBits, Width * Height * 4);
        DestTarget->UnlockRect();
    }

    SafeRelease(RenderTarget);
    SafeRelease(DestTarget);
    return result;
}
