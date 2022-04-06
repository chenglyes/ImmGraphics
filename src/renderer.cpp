#include "renderer.h"

using namespace ImmGraphics;

Renderer::Renderer(RenderDevice* target)
{
    _DB_ASSERT(target && "No avaliable render target.");
    m_target = target;
}

Renderer::~Renderer()
{
    for (auto p : m_pipelines) delete p;
}

void Renderer::AddPipeline(RenderPipeline* pipeline)
{
    _DB_ASSERT(pipeline && "Invalid render pipeline.");
    m_pipelines.PushBack(pipeline);
    pipeline->setRenderTarget(m_target);
}

void Renderer::Render()
{
    for (auto p : m_pipelines)
        p->StartPipeline(m_vertices, m_indices);
    // m_vertices.Resize(0);
    // m_indices.Resize(0);
}

void Renderer::Plane(const Vec3& color)
{
    VertexBuffer vertices = {
        { {0.5f, 0.5f, 0.0f}, color },     // right top
        { {0.5f, -0.5f, 0.0f}, color },    // right bottom
        { {-0.5f, -0.5f, 0.0f}, color },   // left bottom
        { {-0.5f, 0.5f, 0.0f}, color }     // left top
    };

    IndexBuffer indices = {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };

    Mesh(vertices, indices);
}

void Renderer::Box()
{
    // TODO: Add Box Vertex
    
}

void Renderer::Mesh(const VertexBuffer& vertices, const IndexBuffer& indices)
{
    size_t beg = m_vertices.getSize();
    m_vertices.PushBack(vertices);
    for (size_t p : indices)
    {
        m_indices.PushBack(p + beg);
    }
}
