#ifndef __IMMGRAPHICS_HEADER_PIPELINE__
#define __IMMGRAPHICS_HEADER_PIPELINE__

#include "device.h"
#include "debug.h"
#include "uitls.h"

namespace ImmGraphics
{

    class RenderPipeline
    {
    public:
        RenderPipeline(): m_device(nullptr) { }
        virtual ~RenderPipeline() = default;

        virtual void Init() = 0;
        virtual void StartPipeline() = 0;
        virtual void UpdateBuffer() = 0;

        void setRenderTarget(RenderDevice* target) { m_device = target; }

    protected:
        RenderDevice* m_device;

    };

    class ShaderPipeline : public RenderPipeline
    {
    public:
        ShaderPipeline() = default;
        virtual ~ShaderPipeline() = default;

        virtual void Init() {}
        virtual void StartPipeline() { Debug::Print("In Pipeline"); }
        virtual void UpdateBuffer() { Debug::Print("Show Buffer"); }

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_PIPELINE__
