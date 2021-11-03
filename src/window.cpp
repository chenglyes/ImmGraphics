#include "window.h"

#include "platforms/win32.h"

using namespace ImmGraphics;

Window* Window::GenerateWindow(int width, int height)
{
    return new Win32Window(width, height);
}

void Window::DestroyWindow(Window* window)
{
    if (window)
    {
        delete window;
        window = nullptr;
    }
}
