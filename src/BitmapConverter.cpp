//
// Created by marco on 16/11/2024.
//

#include "../includes/BitmapConverter.h"
#include <iostream>
#include <sstream>

std::vector<BYTE> BitmapConverter::HBitmapToRGB(HBITMAP hBitmap, int width, int height) {
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height; // Negative height to avoid flipping
    bi.biPlanes = 1;
    bi.biBitCount = 24; // Use 24-bit RGB
    bi.biCompression = BI_RGB;

    std::vector<BYTE> pixels(width * height * 3);
    HDC hdc = GetDC(nullptr);
    GetDIBits(hdc, hBitmap, 0, height, pixels.data(), reinterpret_cast<BITMAPINFO *>(&bi), DIB_RGB_COLORS);
    ReleaseDC(nullptr, hdc);

    return pixels;
}

std::vector<BYTE> BitmapConverter::ResizeImage(const std::vector<BYTE> &pixels, const int originalWidth,
                                               const int originalHeight, const int newWidth, const int newHeight) {
    std::vector<BYTE> resized(newWidth * newHeight * 3);

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            const int nearestX = static_cast<int>(
                static_cast<float>(x) * (static_cast<float>(originalWidth) / static_cast<float>(newWidth))
            );
            const int nearestY = static_cast<int>(
                static_cast<float>(y) * (static_cast<float>(originalHeight) / static_cast<float>(newHeight))
            );

            const int originalIdx = (nearestY * originalWidth + nearestX) * 3;
            const int resizedIdx = (y * newWidth + x) * 3;

            resized[resizedIdx] = pixels[originalIdx];
            resized[resizedIdx + 1] = pixels[originalIdx + 1];
            resized[resizedIdx + 2] = pixels[originalIdx + 2];
        }
    }

    return resized;
}

std::vector<int> BitmapConverter::CategorizePixels(const std::vector<BYTE> &pixels, int width, int height) {
    std::vector<int> categorizedPixels(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * 3;
            BYTE blue = pixels[idx];
            BYTE green = pixels[idx + 1];
            BYTE red = pixels[idx + 2];

            // convert to grayscale using the luminance formula
            BYTE gray = static_cast<BYTE>(0.299 * red + 0.587 * green + 0.114 * blue);

            // categorize based on grayscale value
            int category;
            if (gray < 63.75) {
                category = 0; // Off
            } else if (gray < 127.5) {
                category = 1; // Red
            } else if (gray < 191.25) {
                category = 3; // Green
            } else {
                category = 2; // Orange
            }

            categorizedPixels[y * width + x] = category;
        }
    }

    return categorizedPixels;
}

std::string BitmapConverter::PixelsToString(const std::vector<int> &pixels) {
    std::ostringstream oss;
    for (const int value: pixels) {
        oss << value;
    }
    return oss.str();
}
