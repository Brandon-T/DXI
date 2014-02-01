#include "Graphics.hpp"

Graphics::Graphics(IDirect3DDevice9* device) : Device(device), Sprite(nullptr), Texture(nullptr), StateBlock(nullptr), VertexBuffer(nullptr) {}

Graphics::~Graphics()
{
    SafeRelease(this->Sprite);
    SafeRelease(this->Texture);
    SafeRelease(this->StateBlock);
    SafeRelease(this->VertexBuffer);
}

void Graphics::SaveState()
{
    if (this->StateBlock)
    {
        this->StateBlock->Release();
        this->StateBlock = nullptr;
    }

    this->Device->CreateStateBlock(D3DSBT_ALL, &this->StateBlock);
    this->StateBlock->Capture();
}

void Graphics::RestoreState()
{
    if (this->StateBlock)
    {
        this->StateBlock->Apply();
        this->StateBlock->Release();
        this->StateBlock = nullptr;
    }
}

void Graphics::SetTextureRenderState()
{
    this->Device->SetRenderState(D3DRS_LIGHTING, false);
    this->Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    this->Device->SetRenderState(D3DRS_ZENABLE, false);
    this->Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    this->Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    this->Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    this->Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void Graphics::SpriteFromFile(const char* FilePath)
{
    this->TextureFromFile(FilePath);
    D3DXCreateSprite(this->Device, &this->Sprite);
}

void Graphics::SpriteFromBuffer(std::uint8_t* Buffer, int Width, int Height)
{
    this->TextureFromBuffer(Buffer, Width, Height);
    D3DXCreateSprite(this->Device, &this->Sprite);
}

void Graphics::TextureFromFile(const char* FilePath)
{
    D3DXCreateTextureFromFileEx(this->Device, FilePath, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0xFF000000, nullptr, nullptr, &this->Texture);
}

void Graphics::TextureFromBuffer(std::uint8_t* Buffer, int Width, int Height)
{
    this->Device->CreateTexture(Width, Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &this->Texture, 0);

    D3DLOCKED_RECT rect;
    this->Texture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
    std::uint8_t* TexturePixels = static_cast<std::uint8_t*>(rect.pBits);
    this->Texture->UnlockRect(0);
    memcpy(&TexturePixels[0], &Buffer[0], Width * Height * 4);
}

void Graphics::CreateVertexBuffer(D3DVertex* Vertices, std::size_t VertexCount, int FVF)
{
    void* pVertices = nullptr;
    this->Device->CreateVertexBuffer(VertexCount * sizeof(D3DVertex), 0, FVF, D3DPOOL_MANAGED, &this->VertexBuffer, nullptr);
    this->VertexBuffer->Lock(0, VertexCount * sizeof(D3DVertex), &pVertices, 0);
    std::memcpy(pVertices, Vertices, VertexCount * sizeof(D3DVertex));
    this->VertexBuffer->Unlock();
}

void Graphics::DrawVertexBuffer(D3DPRIMITIVETYPE PrimitiveType, int PrimitiveCount, int FVF)
{
    this->Device->SetFVF(FVF);
    this->Device->SetStreamSource(0, this->VertexBuffer, 0, sizeof(D3DVertex));
    this->Device->DrawPrimitive(PrimitiveType, 0, PrimitiveCount);
}

void Graphics::DrawRectangleUP(float X1, float Y1, float X2, float Y2, D3DCOLOR Colour)
{
    D3DVertex Vertices[] =
    {
        {X1, Y1, 1.0f, 1.0f, Colour, 0.0f, 0.0f},
        {X2, Y1, 1.0f, 1.0f, Colour, 1.0f, 0.0f},
        {X1, Y2, 1.0f, 1.0f, Colour, 0.0f, 1.0f},
        {X2, Y2, 1.0f, 1.0f, Colour, 1.0f, 1.0f}
    };

    this->Device->SetFVF(FVF_TEX);
    this->Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertices, sizeof(D3DVertex));
}

void Graphics::DrawCircleUP(float CX, float CY, float Radius, D3DCOLOR Colour, int Resolution)
{
    D3DVertex Vertices[Resolution];

    for (int I = 0; I < Resolution; ++I)
    {
        Vertices[I].X = CX + Radius * std::cos(D3DX_PI * (I / (Resolution / 2.0f)));
        Vertices[I].Y = CX + Radius * std::sin(D3DX_PI * (I / (Resolution / 2.0f)));
        Vertices[I].Z = 0.0f;
        Vertices[I].RHW = 1.0f;
        Vertices[I].Colour = Colour;
        Vertices[I].U = 1.0f;
        Vertices[I].V = 1.0f;
    }

    this->Device->SetFVF(FVF_TEX);
    this->Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, Resolution - 2, Vertices, sizeof(D3DVertex));
}
