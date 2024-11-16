//
// Created by marco on 16/11/2024.
//

#ifndef BITMAPCONVERTER_H
#define BITMAPCONVERTER_H

#include <windows.h>
#include <vector>
#include <string>

class BitmapConverter {
public:
    static std::vector<BYTE> HBitmapToRGB(HBITMAP hBitmap, int width, int height);

    static std::vector<BYTE> ResizeImage(const std::vector<BYTE> &pixels, int originalWidth, int originalHeight,
                                         int newWidth, int newHeight);

    static std::vector<int> CategorizePixels(const std::vector<BYTE> &pixels, int width, int height);

    static std::string PixelsToString(const std::vector<int> &pixels);
};

#endif //BITMAPCONVERTER_H
