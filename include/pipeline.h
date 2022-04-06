#ifndef __IMMGRAPHICS_HEADER_PIPELINE__
#define __IMMGRAPHICS_HEADER_PIPELINE__

#include "device.h"
#include "debug.h"
#include "vertex.h"

namespace ImmGraphics
{

    class RenderPipeline
    {
    public:
        RenderPipeline(): m_device(nullptr) { }
        virtual ~RenderPipeline() = default;

        virtual void StartPipeline(const VertexBuffer& vertices, const IndexBuffer& indices) = 0;

        void setRenderTarget(RenderDevice* target) { m_device = target; }

    protected:
        virtual void setBuffer(unsigned x, unsigned y, float z , unsigned value)
        {
            if (x < m_device->width && y < m_device->height)
            {
                float depth = *((float*)m_device->zBuffer + y * m_device->width + x);
                if (z < depth || depth == (float)0xFFFFFFFF)
                {
                    *((unsigned*)m_device->frameBuffer + y * m_device->width + x) = value;
                    *((float*)m_device->zBuffer + y * m_device->width + x) = z;
                }
            }
        }

    protected:
        RenderDevice* m_device;

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_PIPELINE__
