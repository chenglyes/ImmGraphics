#ifndef __IMMGRAPHICS_HEADER_WINDOW__
#define __IMMGRAPHICS_HEADER_WINDOW__

#include "device.h"

namespace ImmGraphics
{

    class RenderDevice;

    class Window
    {
    public:
        virtual ~Window() {}

        virtual void Show() {}
        virtual bool ShouldClose() { return true; }
        virtual void ClearBuffer() {}
        virtual void Draw() {}

        virtual RenderDevice* getDevice() = 0;
        virtual void* getHandle() = 0;

    public:
        static Window* GenerateWindow(int width, int height);
        static void DestroyWindow(Window* window);

    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_WINDOW__
