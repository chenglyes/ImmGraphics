#ifndef __IMMGRAPHICS_HEADER_DEVICE__
#define __IMMGRAPHICS_HEADER_DEVICE__

#include "debug.h"

namespace ImmGraphics
{
    // TODO: RenderDevice
    class RenderDevice
    {
    public:
        int width, height;
        void* frameBuffer;

    public:
        static RenderDevice* CreateDevice(const char* tittle, int weight, int height)
        {
            Debug::Print("Create Device");
            return new RenderDevice;
        }
        static void DestroyDevice(RenderDevice*& device)
        {
            Debug::Print("Destroy Device");
            delete device;
            device = nullptr;
        }
    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_DEVICE__
