#include "platforms/win32.h"
#include "uitls.h"
#include "device.h"

using namespace ImmGraphics;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Window* window = nullptr;

    if (message == WM_CREATE)
    {
        window = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
    }

    switch (message)
    {
    case WM_PAINT:
        if (window) window->Draw();
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }
    
    return 0;
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
}

bool Win32Window::ShouldClose()
{
    if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&m_msg);
        DispatchMessage(&m_msg);
        if (m_msg.message == WM_QUIT) return true;
    }
    return false;
}

void Win32Window::Draw()
{
    BitBlt(m_hDC, 0, 0, m_device.GetWidth(), m_device.GetHeight(), m_hBufferDC, 0, 0, SRCCOPY);
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
    wcex.hbrBackground = NULL;
    wcex.hCursor = (HCURSOR)LoadCursor(nullptr, IDC_ARROW);
    wcex.hIcon = nullptr;
    wcex.hIconSm = wcex.hIcon;
    wcex.hInstance = m_hInstance;
    wcex.style = style;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = m_className;
    wcex.lpfnWndProc = WndProc;

    _DB_ASSERT(RegisterClassEx(&wcex) && "Fail to register window class.");
}

void Win32Window::Create(int width, int height, DWORD style)
{
    int sx, sy;
    int x, y;

    sx = GetSystemMetrics(SM_CXSCREEN);
    sy = GetSystemMetrics(SM_CYSCREEN);

    x = (sx - width) / 2 - width / 1.5f;
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
        LPVOID(this)
    );

    _DB_ASSERT(m_hWnd && "Failed to create win32 window.");
}

void Win32Window::CreateBuffer(int width, int height)
{
    m_device.SetWidth(width);
    m_device.SetHeight(height);

    m_device.GetZBuffer() = new float[width * height]{0};

    m_hDC = GetDC(m_hWnd);
    m_hBufferDC = CreateCompatibleDC(nullptr);

    BITMAPINFOHEADER bmphdr = { 0 };
    bmphdr.biSize = sizeof(bmphdr);
    bmphdr.biWidth = width;
    bmphdr.biHeight = height;
    bmphdr.biPlanes = 1;
    bmphdr.biBitCount = 32;
    bmphdr.biSizeImage = width * height * 4;

    m_hBufferBitmap = CreateDIBSection(
        nullptr,
        (PBITMAPINFO)&bmphdr,
        DIB_RGB_COLORS, (void**)&m_device.GetFrameBuffer(),
        nullptr, 
        0
    );

    _DB_ASSERT(m_hBufferBitmap && "Fail to create buffer.");

    m_hOldBitmap = (HBITMAP)SelectObject(m_hBufferDC, m_hBufferBitmap);

    m_device.ClearBuffer();
}

void Win32Window::ReleaseBuffer()
{
    SelectObject(m_hBufferDC, m_hOldBitmap);
    DeleteDC(m_hBufferDC);
    DeleteObject(m_hOldBitmap);

    m_device.GetFrameBuffer() = nullptr;

    if (m_device.GetZBuffer())
    {
        delete[] m_device.GetZBuffer();
        m_device.GetZBuffer() = nullptr;
    }
}
