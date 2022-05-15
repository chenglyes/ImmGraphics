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
        Renderer();
        ~Renderer();

        void Render(RenderDevice* target, RenderPipeline* pipeline);
        void Clear();

        void Triangle(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& color);
        void Plane(const Vec3& pos, const Vec2& size, const Vec3& color);
        void Box(const Vec3& pos, const Vec3& size, const Vec3& color);
        void Sphere(const Vec3& pos, float radius, int slices, const Vec3& color);
        void Tetrahedron(const Vec3& color);
        void Mesh(const VertexBuffer& vertices, const IndexBuffer& indices);

    private:
        VertexBuffer m_vertices;
        IndexBuffer m_indices;

    };

} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_RENDERER__
