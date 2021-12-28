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

        void ClearBuffer(unsigned color);
        void Plane(unsigned color);
        void Box();
        void Mesh(const VertexBuffer& vertices, const IndexBuffer& indices);

    private:
        RenderDevice* m_target;
        Container<RenderPipeline*> m_pipelines;

        VertexBuffer m_vertices;
        IndexBuffer m_indices;

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_RENDERER__
