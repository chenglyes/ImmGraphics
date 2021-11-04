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
    m_msg = { 0 };

    Register(CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
    Create(width, height, WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);

    RECT rect;
    GetClientRect(m_hWnd, &rect);
    CreateBuffer(rect.right - rect.left, rect.bottom - rect.top);
}

Win32Window::~Win32Window()
{
    ReleaseBuffer();
}

void Win32Window::Show()
{
    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hWnd);
    SetFocus(m_hWnd);
    Draw();
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

void Win32Window::ClearBuffer(unsigned color)
{
    for (unsigned i = 0; i < m_device.width * m_device.height; ++i)
    {
        *((unsigned*)m_device.frameBuffer + i) = color;
        *((float*)m_device.zBuffer + i) = 0;
    }
}

void Win32Window::Draw()
{
    BitBlt(m_hDC, 0, 0, m_device.width, m_device.height, m_hBufferDC, 0, 0, SRCCOPY);
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
    wcex.hCursor = (HCURSOR)LoadCursor(nullptr, IDC_ARROW);
    wcex.hIcon = nullptr;
    wcex.hIconSm = wcex.hIcon;
    wcex.hInstance = m_hInstance;
    wcex.style = style;
    wcex.lpszMenuName = nullptr;
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

void Win32Window::CreateBuffer(int width, int height)
{
    m_device.width = width;
    m_device.height = height;

    m_device.zBuffer = (void*)new float[width * height] { 0 };

    m_hDC = GetDC(m_hWnd);
    m_hBufferDC = CreateCompatibleDC(nullptr);

    BITMAPINFOHEADER bmphdr = { 0 };
    bmphdr.biSize = sizeof(bmphdr);
    bmphdr.biWidth = width;
    bmphdr.biHeight = -height;
    bmphdr.biPlanes = 1;
    bmphdr.biBitCount = 32;
    bmphdr.biSizeImage = width * height * 4;

    m_hBufferBitmap = CreateDIBSection(
        nullptr,
        (PBITMAPINFO)&bmphdr,
        DIB_RGB_COLORS, &m_device.frameBuffer,
        nullptr, 
        0
    );

    _ASSERT(m_hBufferBitmap && "Fail to create buffer.");

    m_hOldBitmap = (HBITMAP)SelectObject(m_hBufferDC, m_hBufferBitmap);
}

void Win32Window::ReleaseBuffer()
{
    SelectObject(m_hBufferDC, m_hOldBitmap);
    DeleteDC(m_hBufferDC);
    DeleteObject(m_hOldBitmap);

    if (m_device.frameBuffer)
    {
        delete[] (unsigned*)m_device.frameBuffer;
        m_device.frameBuffer = nullptr;
    }
    if (m_device.zBuffer)
    {
        delete[] (float*)m_device.zBuffer;
        m_device.zBuffer = nullptr;
    }
}
