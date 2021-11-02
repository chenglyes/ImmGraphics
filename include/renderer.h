#ifndef __IMMGRAPHICS_HEADER_RENDERER__
#define __IMMGRAPHICS_HEADER_RENDERER__

#include "gmath.h"
#include "pipeline.h"
#include "uitls.h"
#include "debug.h"

namespace ImmGraphics
{
    // TODO: Renderer
    class Renderer
    {
    public:
        Renderer(RenderDevice* target): m_pipelines(32)
            { _ASSERT(target && "No avaliable render target."); m_target = target; }
        ~Renderer()
        {
            for (auto p : m_pipelines)
            {
                delete p;
            }
        }

        void AddPipeline(RenderPipeline* pipeline)
        {
            _ASSERT(pipeline && "Invalid render pipeline.");
            m_pipelines.PushBack(pipeline);
            pipeline->setRenderTarget(m_target);
            Debug::Print("Add Pipeline");
        }

        void Line()
        {

        }

        void Plane()
        {

        }

        void Triangle()
        {

        }

        void Box(const Transform& transform)
        {
            Debug::Print("Add Box Vertex");
            doPipelines();
        }

        void Sphere()
        {

        }

        void Mesh()
        {

        }

    private:
        void doPipelines()
        {
            for (auto p : m_pipelines)
            {
                p->StartPipeline();
                p->UpdateBuffer();
            }
        }

    private:
        RenderDevice* m_target;
        Container<RenderPipeline*> m_pipelines;

    };

} // namespace ImmGraphics


#endif // !__IMMGRAPHICS_HEADER_RENDERER__
