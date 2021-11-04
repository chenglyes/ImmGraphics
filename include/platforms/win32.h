#ifndef __IMMGARAPHICS_HEADER_PLATFORMS_WIN32__
#define __IMMGARAPHICS_HEADER_PLATFORMS_WIN32__

#include "../window.h"
#include <windows.h>

namespace ImmGraphics
{
    
    class Win32Window : public Window
    {
    public:
        Win32Window(int width, int height);
        virtual ~Win32Window() override;

        virtual void Show() override;
        virtual bool ShouldClose() override;
        virtual void ClearBuffer(unsigned color) override;
        virtual void Draw() override;

        virtual RenderDevice* getDevice() override;
        virtual void* getHandle() override;

    private:
        void Register(UINT style);
        void Create(int width, int height, DWORD style);
        void CreateBuffer(int width, int height);
        void ReleaseBuffer();

    private:
        RenderDevice m_device;

        LPCSTR m_className;
        HWND m_hWnd;
        HINSTANCE m_hInstance;
        MSG m_msg;

        HDC m_hDC;
        HDC m_hBufferDC;
        HBITMAP m_hBufferBitmap;
        HBITMAP m_hOldBitmap;
        
    };

} // namespace ImmGraphics


#endif // !__IMMGARAPHICS_HEADER_PLATFORMS_WIN32__
