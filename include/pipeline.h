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
        RenderDevice* m_device;

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_PIPELINE__
