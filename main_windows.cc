#include <windows.h>

#include "Backend_Win32_Gdi.h"

static RECT rct;

static LARGE_INTEGER liStart;
static LARGE_INTEGER liStop;

static DWORD height(RECT rect)
{
    return rect.bottom - rect.top;
}

static DWORD width(RECT rect)
{
    return rect.right - rect.left;
}

static void Draw(Starfield &starfield, HDC dc, size_t delta_ns)
{
    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM  = CreateCompatibleBitmap(dc, width(rct), height(rct));
    SelectObject(memDC, memBM);

    Gdi gdi(memDC, width(rct), height(rct));
    render(starfield, gdi, delta_ns);

    BitBlt(dc, 0, 0, width(rct), height(rct), memDC, 0, 0, SRCCOPY);

    DeleteObject(memBM);
    DeleteDC(memDC);
}

static LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch(msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        GetClientRect(hwnd, &rct);
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    LPCWSTR CLASS_NAME = L"C3DE";

    WNDCLASSW wc = {
        .lpfnWndProc = WindowProc,
        .hInstance = hInstance,
        .hCursor = LoadCursorW(NULL, IDC_ARROW),
        .lpszClassName = CLASS_NAME,
    };
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"C3DE",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (hwnd == NULL) {
        return -1;
    }

    HDC dc = GetDC(hwnd);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    if(!QueryPerformanceCounter(&liStart)) {
        MessageBoxW(hwnd, L"Couldn't get the initial time.", L"Error", 0);
        return -3;
    }

    Starfield starfield(4096, 64.0, 20.0);

    MSG msg = {};
    while (true) {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            if(!QueryPerformanceCounter(&liStop)) {
                MessageBoxW(hwnd, L"Couldn't get the step time.", L"Error", 0);
                return -4;
            }
            size_t delta_ns = liStop.QuadPart - liStart.QuadPart;
            liStart = liStop;
            Draw(starfield, dc, delta_ns);
            Sleep(5);
        }
    }

    return 0;
}