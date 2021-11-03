#include "platforms/win32.h"
#include "uitls.h"
#include "device.h"

using namespace ImmGraphics;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    
    return DefWindowProc(hwnd, message, wParam, lParam);
}

Win32Window::Win32Window(int width, int height)
{
    m_hInstance = GetModuleHandle(NULL);
    m_className = TEXT("ImmGraphics Win32 Window");

    Register(CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
    Create(width, height, WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);

    m_msg = { 0 };
    m_device.frameBuffer = malloc(width * height * 4);
    m_device.width = width;
    m_device.height = height;
}

Win32Window::~Win32Window()
{
    if (m_device.frameBuffer) free(m_device.frameBuffer);
}

void Win32Window::Show()
{
    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hWnd);
    SetFocus(m_hWnd);
}

bool Win32Window::ShouldClose()
{
    if (m_msg.message == WM_QUIT) return true;
    if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&m_msg);
        DispatchMessage(&m_msg);
    }
    else
    {
        Sleep(1000 / 60);
    }
    return false;
}

RenderDevice* Win32Window::getDevice()
{
    return &m_device;
}

void* Win32Window::getHandle()
{
    return m_hWnd;
}

void Win32Window::Register(UINT style)
{
    WNDCLASSEX wcex = { 0 };
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.cbSize = sizeof(wcex);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
    wcex.hIcon = NULL;
    wcex.hIconSm = wcex.hIcon;
    wcex.hInstance = m_hInstance;
    wcex.style = style;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_className;
    wcex.lpfnWndProc = WndProc;

    _ASSERT(RegisterClassEx(&wcex) && "Fail to register window class.");
}

void Win32Window::Create(int width, int height, DWORD style)
{
    int sx, sy;
    int x, y;

    sx = GetSystemMetrics(SM_CXSCREEN);
    sy = GetSystemMetrics(SM_CYSCREEN);

    x = (sx - width) / 2;
    y = (sy - height) / 2;

    m_hWnd = CreateWindowEx(
        0,
        m_className,
        m_className,
        style,
        x, y, width, height,
        nullptr,
        nullptr,
        m_hInstance,
        nullptr
    );

    _ASSERT(m_hWnd && "Failed to create win32 window.");
}