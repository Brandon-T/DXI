#ifndef GRAPHICS_HPP_INCLUDED
#define GRAPHICS_HPP_INCLUDED

#include <d3d9.h>
#include <d3dx9.h>
#include <cstring>
#include <cstdint>
#include <cmath>

template<typename T>
void SafeRelease(T*& ptr)
{
    if (ptr)
    {
        ptr->Release();
        ptr = nullptr;
    }
}

struct D3DVertex
{
    float X, Y, Z, RHW;
    unsigned int Colour;
    float U, V;
};

class Graphics
{
    private:
        IDirect3DDevice9* Device;

        ID3DXSprite* Sprite;
        IDirect3DTexture9* Texture;
        IDirect3DStateBlock9* StateBlock;
        IDirect3DVertexBuffer9* VertexBuffer;

    public:
        const static int FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
        const static int FVF_TEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

        Graphics(IDirect3DDevice9* device);
        ~Graphics();

        void SaveState();
        void RestoreState();
        void SetTextureRenderState();

        void SpriteFromFile(const char* FilePath);
        void SpriteFromBuffer(std::uint8_t* Buffer, int Width, int Height);

        void TextureFromFile(const char* FilePath);
        void TextureFromBuffer(std::uint8_t* Buffer, int Width, int Height);

        void CreateVertexBuffer(D3DVertex* Vertices, std::size_t VertexCount, int FVF = FVF_TEX);
        void DrawVertexBuffer(D3DPRIMITIVETYPE PrimitiveType, int PrimitiveCount, int FVF = FVF_TEX);

        void DrawRectangleUP(float X1, float Y1, float X2, float Y2, D3DCOLOR Colour = D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF));
        void DrawCircleUP(float CX, float CY, float Radius, D3DCOLOR Colour = D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), int Resolution = 10);
};

#endif // GRAPHICS_HPP_INCLUDED
