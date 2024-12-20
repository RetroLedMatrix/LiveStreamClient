//
// Created by Katie on 15/11/2024.
//

#ifndef BRAINSBEHINDSCREENSHOT_H
#define BRAINSBEHINDSCREENSHOT_H

#include <windows.h>
#include <string>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

class ScreenCapture {
public:
    ScreenCapture();

    ~ScreenCapture();

    static bool CaptureScreen(const std::wstring &filename);

    static HBITMAP CaptureScreenBitmap(int &width, int &height);

private:
    ULONG_PTR gdiplusToken{};

    void InitializeGDIPlus();

    void ShutdownGDIPlus() const;

    static void SaveBitmapToFile(HBITMAP hBitmap, const std::wstring &filename);

    static int GetEncoderClsid(const WCHAR *format, CLSID *pClsid);
};
#endif //BRAINSBEHINDSCREENSHOT_H
