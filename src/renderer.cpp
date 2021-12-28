#include "renderer.h"

using namespace ImmGraphics;

Renderer::Renderer(RenderDevice* target)
{
    _ASSERT(target && "No avaliable render target.");
    m_target = target;
}

Renderer::~Renderer()
{
    for (auto p : m_pipelines) delete p;
}

void Renderer::AddPipeline(RenderPipeline* pipeline)
{
    _ASSERT(pipeline && "Invalid render pipeline.");
    m_pipelines.PushBack(pipeline);
    pipeline->setRenderTarget(m_target);
}

void Renderer::Render()
{
    for (auto p : m_pipelines)
        p->StartPipeline(m_vertices, m_indices);
    m_vertices.Resize(0);
    m_indices.Resize(0);
}

void Renderer::Plane(unsigned color)
{
    VertexBuffer vertices = {
        { {0.5f, 0.5f, 0.0f}, color },     // 右上角
        { {0.5f, -0.5f, 0.0f}, color },    // 右下角
        { {-0.5f, -0.5f, 0.0f}, color },   // 左下角
        { {-0.5f, 0.5f, 0.0f}, color }     // 左上角
    };

    IndexBuffer indices = {
        0, 1, 3,    // 第一个三角形
        1, 2, 3     // 第二个三角形
    };

    Mesh(vertices, indices);
}

void Renderer::Box()
{
    // TODO: Add Box Vertex
    
}

void Renderer::Mesh(const VertexBuffer& vertices, const IndexBuffer& indices)
{
    unsigned beg = m_vertices.getSize();
    m_vertices.PushBack(vertices);
    for (unsigned p : indices)
    {
        m_indices.PushBack(p + beg);
    }
}
