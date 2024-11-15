#include "ScreenCapture.h"
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

void ScreenCapture::ShutdownGDIPlus() {
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
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC, hBitmap);

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

void ScreenCapture::SaveBitmapToFile(HBITMAP hBitmap, const std::wstring& filename) {
    Gdiplus::Bitmap bitmap(hBitmap, nullptr);
    CLSID clsid;
    if (GetEncoderClsid(L"image/png", &clsid) == 0) {
        bitmap.Save(filename.c_str(), &clsid, nullptr);
    } else {
        std::wcerr << L"Failed to find PNG encoder." << std::endl;
    }
}

int ScreenCapture::GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;          // Number of image encoders
    UINT size = 0;         // Size of the image encoder array in bytes

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;  // Failure

    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == nullptr) return -1;  // Failure

    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
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
