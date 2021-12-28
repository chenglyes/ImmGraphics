#include "pipelines/shaderpip.h"

using namespace ImmGraphics;

ShaderPipeline::ShaderPipeline()
{

}

ShaderPipeline::~ShaderPipeline()
{
    for (auto p : m_shaders) delete p;
}

void ShaderPipeline::StartPipeline(const VertexBuffer &vertices, const IndexBuffer &indices)
{
    VertexBuffer vbo = vertices;

    // vertex shader
    for (auto shader : m_shaders)
        for (auto &v : vbo)
            shader->VSMain(v);

    // primitive assembly


    // rasterize


    // pixel shader
    for (unsigned i = 0; i < m_device->width; ++i)
        for (unsigned j = 0; j < m_device->height; ++j)
        {
            for (auto shader : m_shaders)
                setBuffer(i, j, shader->PSMain());
        }

}

void ShaderPipeline::AddShader(Shader* shader)
{
    _ASSERT(shader && "Invalid shader.");
    m_shaders.PushBack(shader);
}

