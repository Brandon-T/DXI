#ifndef IMAGES_HPP_INCLUDED
#define IMAGES_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <windows.h>

namespace CG
{
    typedef union RGBA
    {
        std::uint32_t Colour;
        struct
        {
            std::uint8_t R, G, B, A;
        };
    } *PRGB;

    class Image
    {
        private:
            RGBA* Pixels;
            std::uint32_t width, height;
            std::uint16_t BitsPerPixel;
            bool PointerInit = false;
#if defined _WIN32 || defined _WIN64
            HBITMAP hBmp = nullptr;
#endif

        protected:
            void ProcessPixels(const std::uint8_t* In, RGBA* Out);
            void ProcessPixels(const RGBA* In, std::uint8_t* Out);

        public:
            explicit Image(const char* FilePath);
            explicit Image(int Width, int Height);
            explicit Image(RGBA* Ptr, int Width, int Height);
            ~Image();

#if defined _WIN32 || defined _WIN64
            explicit Image(HDC DC, int X, int Y, int Width, int Height);
            explicit Image(HWND Window, int X, int Y, int Width, int Height);
            void Draw(HWND Hwnd, int X, int Y, int Width, int Height, int SourceX, int SourceY, DWORD RasterType);
            void Draw(HDC hDC, int X, int Y, int Width, int Height, int SourceX, int SourceY, DWORD RasterType);
#endif

            void Save(const char* FilePath);

            inline std::uint16_t Bpp()
            {
                return BitsPerPixel;
            }

            inline int Width() const
            {
                return width;
            }

            inline int Height() const
            {
                return height;
            }

            inline RGBA* GetPixels()
            {
                return Pixels;
            }

            inline RGBA GetPixel(int X, int Y) const
            {
                return Pixels[Y * width + X];
            }

            inline void SetPixel(int X, int Y, std::uint32_t Color)
            {
                Pixels[Y * width + X].Colour = Color;
            }
    };
}

#endif // IMAGES_HPP_INCLUDED
