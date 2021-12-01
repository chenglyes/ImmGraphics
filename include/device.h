#ifndef __IMMGRAPHICS_HEADER_DEVICE__
#define __IMMGRAPHICS_HEADER_DEVICE__

namespace ImmGraphics
{

    class RenderDevice
    {
    public:
        unsigned width, height;
        void* frameBuffer;
        void* zBuffer;

    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_DEVICE__
