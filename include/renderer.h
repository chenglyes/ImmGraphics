#ifndef __IMMGRAPHICS_HEADER_RENDERER__
#define __IMMGRAPHICS_HEADER_RENDERER__

#include "gmath.h"
#include "pipeline.h"
#include "uitls.h"

namespace ImmGraphics
{

    class Renderer
    {
    public:
        Renderer(RenderDevice* target);
        ~Renderer();

        void AddPipeline(RenderPipeline* pipeline);
        void Render();

        void Box(const Transform& transform);

    private:
        void doPipelines();
        void updateBuffer();

    private:
        RenderDevice* m_target;
        Container<RenderPipeline*> m_pipelines;

    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_RENDERER__
