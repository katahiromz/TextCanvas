// (in Japanese Shift_JIS)
#include <windows.h>
#include <windowsx.h>
#include <cassert>
#include <cstdio>
#include "japanese.hpp"

typedef struct tagBITMAPINFOEX
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[256];
} BITMAPINFOEX, FAR * LPBITMAPINFOEX;

BOOL SaveBitmapToFile(LPCTSTR pszFileName, HBITMAP hbm)
{
    BOOL f;
    BITMAPFILEHEADER bf;
    BITMAPINFOEX bi;
    BITMAPINFOHEADER *pbmih;
    DWORD cb;
    DWORD cColors, cbColors;
    HDC hDC;
    HANDLE hFile;
    LPVOID pBits;
    BITMAP bm;
    DWORD dwError = 0;

    if (!GetObject(hbm, sizeof(BITMAP), &bm))
        return FALSE;

    pbmih = &bi.bmiHeader;
    ZeroMemory(pbmih, sizeof(BITMAPINFOHEADER));
    pbmih->biSize             = sizeof(BITMAPINFOHEADER);
    pbmih->biWidth            = bm.bmWidth;
    pbmih->biHeight           = bm.bmHeight;
    pbmih->biPlanes           = 1;
    pbmih->biBitCount         = bm.bmBitsPixel;
    pbmih->biCompression      = BI_RGB;
    pbmih->biSizeImage        = bm.bmWidthBytes * bm.bmHeight;

    if (bm.bmBitsPixel < 16)
        cColors = 1 << bm.bmBitsPixel;
    else
        cColors = 0;
    cbColors = cColors * sizeof(RGBQUAD);

    bf.bfType = 0x4d42;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    cb = sizeof(BITMAPFILEHEADER) + pbmih->biSize + cbColors;
    bf.bfOffBits = cb;
    bf.bfSize = cb + pbmih->biSizeImage;

    pBits = HeapAlloc(GetProcessHeap(), 0, pbmih->biSizeImage);
    if (pBits == NULL)
        return FALSE;

    f = FALSE;
    hDC = GetDC(NULL);
    if (hDC != NULL)
    {
        if (GetDIBits(hDC, hbm, 0, bm.bmHeight, pBits, (BITMAPINFO*)&bi,
            DIB_RGB_COLORS))
        {
            hFile = CreateFile(pszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL |
                               FILE_FLAG_WRITE_THROUGH, NULL);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                f = WriteFile(hFile, &bf, sizeof(BITMAPFILEHEADER), &cb, NULL) &&
                    WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &cb, NULL) &&
                    WriteFile(hFile, bi.bmiColors, cbColors, &cb, NULL) &&
                    WriteFile(hFile, pBits, pbmih->biSizeImage, &cb, NULL);
                if (!f)
                    dwError = GetLastError();
                CloseHandle(hFile);

                if (!f)
                    DeleteFile(pszFileName);
            }
            else
                dwError = GetLastError();
        }
        else
            dwError = GetLastError();
        ReleaseDC(NULL, hDC);
    }
    else
        dwError = GetLastError();

    HeapFree(GetProcessHeap(), 0, pBits);
    SetLastError(dwError);
    return f;
}

HBITMAP Create24BppBitmap(HDC hdc, INT cx, INT cy)
{
    BITMAPINFO bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = cx;
    bi.bmiHeader.biHeight = cy;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 24;
    LPVOID pvBits;
    return CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &pvBits, NULL, 0);
}

int main(void)
{
    if (HDC hdc = CreateCompatibleDC(NULL))
    {
        LOGFONTA lf;
        ZeroMemory(&lf, sizeof(lf));
        lf.lfHeight = -16;
        lf.lfCharSet = SHIFTJIS_CHARSET;
        lstrcpyA(lf.lfFaceName, "KHドット小伝馬町16 幾何学的カナ");
        if (HFONT hFont = CreateFontIndirectA(&lf))
        {
            HGDIOBJ hFontOld = SelectObject(hdc, hFont);
            {
                if (HBITMAP hbm = Create24BppBitmap(hdc, 16 * 8, 16 * 16))
                {
                    HGDIOBJ hbmOld = SelectObject(hdc, hbm);
                    for (INT iy = 0; iy < 16; ++iy)
                    {
                        INT y = iy * 16;
                        for (INT ix = 0; ix <= 16; ++ix)
                        {
                            INT x = ix * 8;

                            SetTextColor(hdc, RGB(255, 255, 255));
                            SetBkColor(hdc, RGB(0, 0, 0));
                            SetBkMode(hdc, OPAQUE);

                            char ch = ix | (iy << 4);

                            TextOutA(hdc, x, y, &ch, 1);
                        }
                    }
                    SelectObject(hdc, hbmOld);
                    SaveBitmapToFile("KH-Dot-Kodenmachou-16-Ki-ASCII.bmp", hbm);
                    DeleteObject(hbm);
                }
            }
            SelectObject(hdc, hFontOld);
            DeleteObject(hFont);
        }
        DeleteDC(hdc);
    }
    return 0;
}
