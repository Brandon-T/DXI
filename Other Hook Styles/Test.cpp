#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

int WindowWidth = 500;
int WindowHeight = 500;

IDirect3D9* d3dinterface = nullptr;
IDirect3DDevice9* d3ddevice = nullptr;
IDirect3DTexture9* texture = nullptr;
IDirect3DVertexBuffer9* vertexbuffer = nullptr;

#define VERTEX_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define VERTEX_FVF_TEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct D3DVertex
{
    float X, Y, Z, RHW;
    unsigned int Colour;
    float U, V;
};

template<typename T>
void SafeRelease(T*& ptr)
{
    if (ptr)
    {
        ptr->Release();
        ptr = nullptr;
    }
}

void EnableDrawing(IDirect3DStateBlock9*& block)
{
    d3ddevice->CreateStateBlock(D3DSBT_ALL, &block);
    block->Capture();
}

void DisableDrawing(IDirect3DStateBlock9*& block)
{
    if (block)
    {
        block->Apply();
        block->Release();
        block = nullptr;
    }
}

void DrawVertexBuffer(float X1, float Y1, float X2, float Y2)
{
    if (!vertexbuffer)
    {
        D3DVertex vertices[] =
        {
            {X1, Y1, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 0.0f, 0.0f},
            {X2, Y1, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 1.0f, 0.0f},
            {X1, Y2, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 0.0f, 1.0f},
            {X2, Y2, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 1.0f, 1.0f}
        };

        void* pVertices = nullptr;
        d3ddevice->CreateVertexBuffer(sizeof(vertices), 0, VERTEX_FVF_TEX, D3DPOOL_MANAGED, &vertexbuffer, nullptr);
        vertexbuffer->Lock(0, sizeof(vertices), &pVertices, 0);
        memcpy(pVertices, vertices, sizeof(vertices));
        vertexbuffer->Unlock();
    }

    d3ddevice->SetFVF(VERTEX_FVF_TEX);
    d3ddevice->SetStreamSource(0, vertexbuffer, 0, sizeof(D3DVertex));
    d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void DrawTexture(float X1, float Y1, float X2, float Y2)
{
    D3DVertex vertices[] =
    {
        {X1, Y1, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 0.0f, 0.0f},
        {X2, Y1, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 1.0f, 0.0f},
        {X1, Y2, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 0.0f, 1.0f},
        {X2, Y2, 1.0f, 1.0f, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), 1.0f, 1.0f}
    };

    d3ddevice->SetRenderState(D3DRS_LIGHTING, false);
    d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    d3ddevice->SetRenderState(D3DRS_ZENABLE, false);
    d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    d3ddevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    d3ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    d3ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    d3ddevice->SetFVF(VERTEX_FVF_TEX);
    d3ddevice->SetTexture(0, texture);
    d3ddevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DVertex));
}

void DrawCircle(float mx, float my, float r, D3DCOLOR colour)
{
    static const int CIRCLE_RESOLUTION = 10;
    D3DVertex verts[CIRCLE_RESOLUTION];

    for (int i = 0; i < CIRCLE_RESOLUTION; ++i)
    {
        verts[i].X = mx + r * cos(D3DX_PI * (i / (CIRCLE_RESOLUTION / 2.0f)));
        verts[i].Y = my + r * sin(D3DX_PI * (i / (CIRCLE_RESOLUTION / 2.0f)));
        verts[i].Z = 0.0f;
        verts[i].RHW = 1.0f;
        verts[i].Colour = colour;
        verts[i].U = 1.0f;
        verts[i].V = 1.0f;
    }

    d3ddevice->SetFVF(VERTEX_FVF);
    d3ddevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, CIRCLE_RESOLUTION - 2, verts, sizeof(D3DVertex));
}

POINT P;
void RenderD3D()
{
    d3ddevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
    d3ddevice->BeginScene();

        IDirect3DStateBlock9* block = nullptr;
        EnableDrawing(block);

        d3ddevice->SetRenderState(D3DRS_LIGHTING, false);
        d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        d3ddevice->SetRenderState(D3DRS_ZENABLE, false);
        d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        d3ddevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        d3ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        d3ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        DrawCircle(P.x, P.y, 2.5, D3DCOLOR_RGBA(0xFF, 0x00, 0x00, 0xFF));
        DisableDrawing(block);

    d3ddevice->EndScene();
    d3ddevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void LoadTexture(const char* FilePath, IDirect3DTexture9*& Texture)
{
    D3DXCreateTextureFromFileEx(d3ddevice, FilePath, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0xFF000000, nullptr, nullptr, &Texture);
}

void InitD3D(HWND WindowHandle)
{
    D3DPRESENT_PARAMETERS d3dpp = {0};
    memset(&d3dpp, 0, sizeof(d3dpp));

    d3dpp.Windowed          = true;
    d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow     = WindowHandle;
    d3dpp.BackBufferWidth   = WindowWidth;
    d3dpp.BackBufferHeight  = WindowHeight;
    d3dpp.BackBufferFormat  = D3DFMT_A8R8G8B8;

    d3dinterface = Direct3DCreate9(D3D_SDK_VERSION);
    d3dinterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, WindowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddevice);

    /*D3DXMATRIX World, View, Projection;
    D3DXVECTOR3 Camera = {0.0f, 0.0f, 10.0f};
    D3DXVECTOR3 LookAt = {0.0f, 0.0f, 0.0f};
    D3DXVECTOR3 UpVector = {0.0f, 1.0f, 0.0f};

    D3DXMatrixIdentity(&World);
    D3DXMatrixLookAtLH(&View, &Camera, &LookAt, &UpVector);
    D3DXMatrixPerspectiveFovLH(&Projection, D3DXToRadian(45), static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight), 1.0f, 100.0f);

    d3ddevice->SetTransform(D3DTS_WORLD, &World);
    d3ddevice->SetTransform(D3DTS_VIEW, &View);
    d3ddevice->SetTransform(D3DTS_PROJECTION, &Projection);
    LoadTexture("C:/Users/School/Desktop/Foo.png", texture);*/
}



void CleanUpD3d()
{
    SafeRelease(texture);
    SafeRelease(vertexbuffer);
    SafeRelease(d3ddevice);
    SafeRelease(d3dinterface);
}

LRESULT __stdcall WindowProcedure(HWND Hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        case WM_MOUSEMOVE:
            P = {LOWORD(lParam), HIWORD(lParam)};
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(Hwnd, Msg, wParam, lParam);
    }
    return 0;
}

int __stdcall WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    WNDCLASSEX WndClass =
    {
        sizeof(WNDCLASSEX), CS_DBLCLKS, WindowProcedure,
        0, 0, GetModuleHandle(nullptr), LoadIcon(nullptr, IDI_APPLICATION),
        LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_BACKGROUND),
        nullptr, "Direct-X", LoadIcon(nullptr, IDI_APPLICATION)
    };

    if(RegisterClassEx(&WndClass))
    {
        HWND WindowHandle = CreateWindow("Direct-X", "Direct-X", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight, nullptr, nullptr, hThisInstance, nullptr);
        ShowWindow(WindowHandle, nCmdShow);
        UpdateWindow(WindowHandle);
        InitD3D(WindowHandle);
    }

    MSG Messages = {0};

    while(true)
    {
        while(PeekMessage(&Messages, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Messages);
            DispatchMessage(&Messages);
        }

        if (Messages.message == WM_QUIT)
            break;

        RenderD3D();
    }

    CleanUpD3d();
    return Messages.wParam;
}
