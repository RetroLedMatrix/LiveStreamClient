//
// Created by Katie on 15/11/2024.
//

#include "../includes/BrainsBehindScreenshot.h"
#include <iostream>

using namespace Gdiplus;

ScreenCapture::ScreenCapture() {
    InitializeGDIPlus();
}

ScreenCapture::~ScreenCapture() {
    ShutdownGDIPlus();
}

void ScreenCapture::InitializeGDIPlus() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

void ScreenCapture::ShutdownGDIPlus() const {
    GdiplusShutdown(gdiplusToken);
}

bool ScreenCapture::CaptureScreen(const std::wstring& filename) {
    // Get screen dimensions
    HDC screenDC = GetDC(nullptr);  // Get the device context of the screen
    HDC memoryDC = CreateCompatibleDC(screenDC);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a compatible bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, screenWidth, screenHeight);

    if (!hBitmap) {
        std::wcerr << L"Failed to create compatible bitmap." << std::endl;
        DeleteDC(memoryDC);
        ReleaseDC(nullptr, screenDC);
        return false;
    }

    // Select the bitmap into the memory DC
    auto oldBitmap = (HBITMAP)SelectObject(memoryDC, hBitmap);

    // Copy screen content to the bitmap
    BitBlt(memoryDC, 0, 0, screenWidth, screenHeight, screenDC, 0, 0, SRCCOPY);

    // Save the bitmap to a file
    SaveBitmapToFile(hBitmap, filename);

    // Clean up
    SelectObject(memoryDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memoryDC);
    ReleaseDC(nullptr, screenDC);

    return true;
}

// void ScreenCapture::SaveBitmapToFile(HBITMAP hBitmap, const std::wstring& filename) {
//     Gdiplus::Bitmap bitmap(hBitmap, nullptr);
//     CLSID clsid;
//     if (GetEncoderClsid(L"image/png", &clsid) == 0) {
//         bitmap.Save(filename.c_str(), &clsid, nullptr);
//     } else {
//         std::wcerr << L"Failed to find PNG encoder." << std::endl;
//     }
// }

void ScreenCapture::SaveBitmapToFile(HBITMAP hBitmap, const std::wstring& filename) {
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    // Initialize the BITMAPINFOHEADER
    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = bmp.bmWidth;
    infoHeader.biHeight = -bmp.bmHeight; // Negative height to ensure top-down DIB
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = bmp.bmBitsPixel;
    infoHeader.biCompression = BI_RGB;
    infoHeader.biSizeImage = 0;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    // Calculate the size of the pixel data
    DWORD bitmapDataSize = ((bmp.bmWidth * infoHeader.biBitCount + 31) / 32) * 4 * bmp.bmHeight;

    // Set up the BITMAPFILEHEADER
    fileHeader.bfType = 0x4D42; // 'BM' in little-endian
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bitmapDataSize;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Allocate memory for the bitmap data
    const auto bitmapData = new char[bitmapDataSize];
    const HDC hdc = GetDC(nullptr);

    // Retrieve the bitmap data
    if (!GetDIBits(hdc, hBitmap, 0, bmp.bmHeight, bitmapData, (BITMAPINFO*)&infoHeader, DIB_RGB_COLORS)) {
        std::wcerr << L"Failed to retrieve bitmap data." << std::endl;
        delete[] bitmapData;
        ReleaseDC(nullptr, hdc);
        return;
    }

    // Write the bitmap to the file
    HANDLE fileHandle = CreateFileW(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (fileHandle != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        WriteFile(fileHandle, &fileHeader, sizeof(BITMAPFILEHEADER), &bytesWritten, nullptr);
        WriteFile(fileHandle, &infoHeader, sizeof(BITMAPINFOHEADER), &bytesWritten, nullptr);
        WriteFile(fileHandle, bitmapData, bitmapDataSize, &bytesWritten, nullptr);
        CloseHandle(fileHandle);
    } else {
        std::wcerr << L"Failed to create file: " << filename << std::endl;
    }

    // Clean up
    delete[] bitmapData;
    ReleaseDC(nullptr, hdc);
}



int ScreenCapture::GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;          // Number of image encoders
    UINT size = 0;         // Size of the image encoder array in bytes

    GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;  // Failure

    auto* pImageCodecInfo = static_cast<ImageCodecInfo *>(malloc(size));
    if (pImageCodecInfo == nullptr) return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);
    for (UINT i = 0; i < num; ++i) {
        if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[i].Clsid;
            free(pImageCodecInfo);
            return 0;  // Success
        }
    }
    free(pImageCodecInfo);
    return -1;  // Failure
}
