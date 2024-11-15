#include <windows.h>
#include <iostream>
#include <string>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// Forward declaration of the GetEncoderClsid function
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

void SaveBitmapToFile(HBITMAP hBitmap, const std::wstring& filename) {
    Gdiplus::Bitmap bitmap(hBitmap, nullptr);
    CLSID clsid;
    // Get the CLSID of the PNG encoder
    if (GetEncoderClsid(L"image/png", &clsid) == 0) {
        bitmap.Save(filename.c_str(), &clsid, nullptr);
    } else {
        std::wcerr << L"Failed to find PNG encoder." << std::endl;
    }
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;          // Number of image encoders
    UINT size = 0;         // Size of the image encoder array in bytes

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;  // Failure

    Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
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

int main() {
    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Get screen dimensions
    HDC screenDC = GetDC(nullptr);  // Get the device context of the screen
    HDC memoryDC = CreateCompatibleDC(screenDC);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a compatible bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, screenWidth, screenHeight);

    // Select the bitmap into the memory DC
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC, hBitmap);

    // Copy screen content to the bitmap
    BitBlt(memoryDC, 0, 0, screenWidth, screenHeight, screenDC, 0, 0, SRCCOPY);

    // Save the bitmap to a file
    std::wstring filename = L"screenshot.png";
    SaveBitmapToFile(hBitmap, filename);

    // Clean up
    SelectObject(memoryDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memoryDC);
    ReleaseDC(nullptr, screenDC);

    // Shutdown GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    std::wcout << L"Screenshot saved to " << filename << std::endl;
    return 0;
}
