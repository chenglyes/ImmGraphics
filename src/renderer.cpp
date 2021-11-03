#include "renderer.h"
#include "debug.h"

using namespace ImmGraphics;

Renderer::Renderer(RenderDevice* target): m_pipelines(32)
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
    Debug::Print("Add Pipeline");
}

void Renderer::Render()
{
    doPipelines();
    updateBuffer();
}

void Renderer::Box(const Transform& transform)
{
    Debug::Print("Add Box Vertex");
}

void Renderer::doPipelines()
{
    for (auto p : m_pipelines) p->StartPipeline();
}

void Renderer::updateBuffer()
{
    for (auto p : m_pipelines) p->UpdateBuffer();
}
