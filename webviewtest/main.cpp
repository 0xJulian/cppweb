#include <windows.h>
#include "webview.h"
#include <dwmapi.h>

#pragma comment(lib, "Dwmapi.lib")

int WINAPI wWinMain(const HINSTANCE hInstance, HINSTANCE, PWSTR, const int nShowCmd)
{
    constexpr wchar_t className[] = L"WebView2 Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = window_proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
    {
        return 0;
    }

    const HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED,
        className,
        L"WebView2 Sample",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr)
    {
        return 0;
    }

    constexpr int opacity = 90;
    SetLayeredWindowAttributes(hwnd, 0, (255 * opacity) / 100, LWA_ALPHA);

    // Enable dark mode
    constexpr BOOL useDarkMode = true;
    SUCCEEDED(DwmSetWindowAttribute(
        hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &useDarkMode, sizeof(useDarkMode)));

    ShowWindow(hwnd, nShowCmd);
    initialize_web_view(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
