#include "Images.hpp"

namespace CG
{
    Image::~Image()
    {
        if (!PointerInit)
            delete[] this->Pixels;
        #if defined _WIN32 || defined _WIN64
        DeleteObject(hBmp);
        #endif
    }

    Image::Image(const char* FilePath) : Pixels(nullptr), width(0), height(0), BitsPerPixel(0)
    {
        std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
        if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

        hFile.seekg(0, std::ios::end);
        int Length = hFile.tellg();
        hFile.seekg(0, std::ios::beg);
        std::vector<std::uint8_t> FileInfo(Length);
        hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

        if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
        {
            hFile.close();
            throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
        }

        if (FileInfo[28] != 24 && FileInfo[28] != 32)
        {
            hFile.close();
            throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
        }

        BitsPerPixel = FileInfo[28];
        width = FileInfo[18] + (FileInfo[19] << 8);
        height = FileInfo[22] + (FileInfo[23] << 8);
        std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
        std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
        std::vector<std::uint8_t> Data(size);

        hFile.seekg (PixelsOffset, std::ios::beg);
        hFile.read(reinterpret_cast<char*>(Data.data()), size);
        hFile.close();

        this->Pixels = new RGBA[width * height];
        this->ProcessPixels(Data.data(), this->Pixels);
    }

    Image::Image(int Width, int Height) : Pixels(new RGBA[Width * Height]), width(Width), height(Height), BitsPerPixel(32)
    {
        std::memset(this->Pixels, 0, Width * Height);
    }

    Image::Image(RGBA* Ptr, int Width, int Height) : Pixels(Ptr), width(Width), height(Height), BitsPerPixel(32)
    {
        PointerInit = true;
    }

    #if defined _WIN32 || defined _WIN64
    Image::Image(HDC DC, int X, int Y, int Width, int Height) : Pixels(nullptr), width(Width), height(Height), BitsPerPixel(32)
    {
        BITMAP Bmp = {0};
        HBITMAP hBmp = reinterpret_cast<HBITMAP>(GetCurrentObject(DC, OBJ_BITMAP));

        if (GetObject(hBmp, sizeof(BITMAP), &Bmp) == 0)
            throw std::runtime_error("BITMAP DC NOT FOUND.");

        RECT area = {X, Y, X + Width, Y + Height};
        HWND Window = WindowFromDC(DC);
        GetClientRect(Window, &area);

        HDC MemDC = GetDC(nullptr);
        HDC SDC = CreateCompatibleDC(MemDC);
        HBITMAP hSBmp = CreateCompatibleBitmap(MemDC, width, height);
        DeleteObject(SelectObject(SDC, hSBmp));

        BitBlt(SDC, 0, 0, width, height, DC, X, Y, SRCCOPY);
        std::vector<std::uint8_t> Data(((width * BitsPerPixel + 31) / 32) * 4 * height);

        BITMAPINFO Info = {sizeof(BITMAPINFOHEADER), width, height, 1, BitsPerPixel, BI_RGB, Data.size(), 0, 0, 0, 0};
        GetDIBits(SDC, hSBmp, 0, height, Data.data(), &Info, DIB_RGB_COLORS);

        DeleteDC(SDC);
        DeleteObject(hSBmp);
        ReleaseDC(nullptr, MemDC);
        this->ProcessPixels(Data.data(), this->Pixels);
    }

    Image::Image(HWND Window, int X, int Y, int Width, int Height)
    {
        HDC DC = GetDC(Window);
        BITMAP Bmp = {0};
        HBITMAP hBmp = reinterpret_cast<HBITMAP>(GetCurrentObject(DC, OBJ_BITMAP));

        if (GetObject(hBmp, sizeof(BITMAP), &Bmp) == 0)
            throw std::runtime_error("BITMAP DC NOT FOUND.");

        RECT area = {X, Y, X + Width, Y + Height};
        GetClientRect(Window, &area);

        HDC MemDC = GetDC(nullptr);
        HDC SDC = CreateCompatibleDC(MemDC);
        HBITMAP hSBmp = CreateCompatibleBitmap(MemDC, width, height);
        DeleteObject(SelectObject(SDC, hSBmp));

        BitBlt(SDC, 0, 0, width, height, DC, X, Y, SRCCOPY);
        std::vector<std::uint8_t> Data(((width * BitsPerPixel + 31) / 32) * 4 * height);

        BITMAPINFO Info = {sizeof(BITMAPINFOHEADER), width, height, 1, BitsPerPixel, BI_RGB, Data.size(), 0, 0, 0, 0};
        GetDIBits(SDC, hSBmp, 0, height, Data.data(), &Info, DIB_RGB_COLORS);

        DeleteDC(SDC);
        DeleteObject(hSBmp);
        ReleaseDC(nullptr, MemDC);
        ReleaseDC(Window, DC);
        this->ProcessPixels(Data.data(), this->Pixels);
    }

    void Image::Draw(HWND Hwnd, int X , int Y , int Width , int Height , int SourceX , int SourceY, DWORD RasterType)
    {
        HDC hDC = GetDC(Hwnd);
        Draw(hDC, X, Y, Width, Height, SourceX, SourceY, RasterType);
        ReleaseDC(Hwnd, hDC);
    }

    void Image::Draw(HDC hDC, int X, int Y, int Width, int Height, int SourceX, int SourceY, DWORD RasterType)
    {
        BITMAP Bmp;
        HDC MemDC = CreateCompatibleDC(hDC);
        if (!hBmp)
        {
            std::vector<std::uint8_t> Data(((width * BitsPerPixel + 31) / 32) * 4 * height);
            BITMAPINFO Info = {sizeof(BITMAPINFOHEADER), width, height, 1, BitsPerPixel, BI_RGB, Data.size(), 0, 0, 0, 0};
            this->ProcessPixels(this->Pixels, Data.data());
            DeleteObject(hBmp);
            hBmp = CreateDIBitmap(GetDC(nullptr), &Info.bmiHeader, CBM_INIT, Data.data(), &Info, DIB_RGB_COLORS);
        }

        HGDIOBJ hbmOld = SelectObject(MemDC, hBmp);
        GetObject(hBmp, sizeof(Bmp), &Bmp);

        BitBlt(hDC, X, Y, Width == 0 ? Bmp.bmWidth : Width, Height == 0 ? Bmp.bmHeight : Height, MemDC, SourceX, SourceY, RasterType);

        SelectObject(MemDC, hbmOld);
        DeleteDC(MemDC);
    }
    #endif

    void Image::Save(const char* FilePath)
    {
        std::fstream hFile(FilePath, std::ios::out | std::ios::binary);
        if (!hFile.is_open()) throw std::invalid_argument("Error: File not found.");

        std::uint32_t Trash = 0;
        std::uint16_t Planes = 1;
        std::uint32_t biSize = 40;
        std::uint16_t Type = 0x4D42;
        std::uint32_t compression = 0;
        std::uint32_t PixelsOffsetBits = 54;
        std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
        std::uint32_t bfSize = 54 + size;
        std::vector<std::uint8_t> Data(size);

        this->ProcessPixels(this->Pixels, Data.data());

        hFile.write(reinterpret_cast<char*>(&Type), sizeof(Type));
        hFile.write(reinterpret_cast<char*>(&bfSize), sizeof(bfSize));
        hFile.write(reinterpret_cast<char*>(&Trash), sizeof(std::uint32_t));
        hFile.write(reinterpret_cast<char*>(&PixelsOffsetBits), sizeof(PixelsOffsetBits));
        hFile.write(reinterpret_cast<char*>(&biSize), sizeof(biSize));
        hFile.write(reinterpret_cast<char*>(&width), sizeof(width));
        hFile.write(reinterpret_cast<char*>(&height), sizeof(height));
        hFile.write(reinterpret_cast<char*>(&Planes), sizeof(Planes));
        hFile.write(reinterpret_cast<char*>(&BitsPerPixel), sizeof(BitsPerPixel));
        hFile.write(reinterpret_cast<char*>(&compression), sizeof(compression));
        hFile.write(reinterpret_cast<char*>(&size), sizeof(size));
        hFile.write(reinterpret_cast<char*>(&Trash), sizeof(std::uint32_t) * 4);
        hFile.write(reinterpret_cast<char*>(Data.data()), size);
        hFile.close();
    }

    void Image::ProcessPixels(const std::uint8_t* In, RGBA* Out)
    {
        for (std::size_t I = 0; I < height; ++I)
        {
            for (std::size_t J = 0; J < width; ++J)
            {
                Out[(height - 1 - I) * width + J].B = *(In++);
                Out[(height - 1 - I) * width + J].G = *(In++);
                Out[(height - 1 - I) * width + J].R = *(In++);
                Out[(height - 1 - I) * width + J].A = (BitsPerPixel > 24 ? * (In++) : 0xFF);
            }
            if(BitsPerPixel == 24)
                In += (-width * 3) & 3;
        }
    }

    void Image::ProcessPixels(const RGBA* In, std::uint8_t* Out)
    {
        for (std::size_t I = 0; I < height; ++I)
        {
            for (std::size_t J = 0; J < width; ++J)
            {
                *(Out++) = In[(height - 1 - I) * width + J].B;
                *(Out++) = In[(height - 1 - I) * width + J].G;
                *(Out++) = In[(height - 1 - I) * width + J].R;

                if (BitsPerPixel > 24)
                    *(Out++) = In[(height - 1 - I) * width + J].A;
            }
            if(BitsPerPixel == 24)
                Out += (-width * 3) & 3;
        }
    }
}
