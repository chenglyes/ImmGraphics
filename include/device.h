#ifndef __IMMGRAPHICS_HEADER_DEVICE__
#define __IMMGRAPHICS_HEADER_DEVICE__

namespace ImmGraphics
{
    // TODO: RenderDevice
    class RenderDevice
    {
    public:
        int width, height;
        void* frameBuffer;

    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_DEVICE__