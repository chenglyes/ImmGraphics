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
    #pragma omp parallel for
    for (int i = 0; i < indices.getSize(); i += 3)
    {
        if (i + 3 > indices.getSize()) break;

        Vec3 index(indices[i], indices[i + 1], indices[i + 2]);

        // render one triangle
        RenderPrimitive(vertices, index);
    }
}

void ShaderPipeline::AddShader(Shader* shader)
{
    _DB_ASSERT(shader && "Invalid shader.");
    m_shaders.PushBack(shader);
}

Vec3 ShaderPipeline::BufferToRelativeTView(const Vec3& position)
{
    Vec3 res;
    res.x = position.x / m_device->width - 0.5;
    res.y = 0.5 - position.y / m_device->height;
    res.z = position.z;
    return res;
}

Vec3 ShaderPipeline::RelativeToBufferView(const Vec3& position)
{
    Vec3 res;
    res.x = (position.x + 0.5) * m_device->width;
    res.y = (0.5 - position.y) * m_device->height;
    res.z = position.z;
    return res;
}

void ShaderPipeline::RenderPrimitive(const VertexBuffer& vertices, const Vec3& index)
{
    for (Shader* shader : m_shaders)
    {
        VaryingData datas[4] = {};

        // vertex shader
        Vec3 a = RelativeToBufferView(shader->VSMain(vertices[index.x], datas[0]));
        Vec3 b = RelativeToBufferView(shader->VSMain(vertices[index.y], datas[1]));
        Vec3 c = RelativeToBufferView(shader->VSMain(vertices[index.z], datas[2]));

        Triangle2D triangle(a, b, c);

        // rasterize
        int lx = Math::Min(a.x, b.x, c.x);
        int rx = Math::Max(a.x, b.x, c.x);
        int ty = Math::Min(a.y, b.y, c.y);
        int by = Math::Max(a.y, b.y, c.y);

#pragma omp parallel for
        for (int x = lx; x < rx; ++x)
        {

            for (int y = ty; y < by; ++y)
            {
                // Handle varying datas
                Vec2 p(x + 0.5, y + 0.5);

                if (!triangle.Contain(p)) continue;

                Vec3 weight = triangle.Interpolation(p);
                HandleVarying(weight, datas);

                // pixel shader
                Vec3 color = shader->PSMain(datas[3]);

                float z = a.z * weight.x + b.z * weight.y + c.z * weight.z;
                setBuffer(x, y, z, Color(color.x, color.y, color.z).getRGBValue());
            }
        }
    }
}

void ShaderPipeline::HandleVarying(const Vec3& weight, VaryingData* datas)
{
    for (const auto& ds : datas[0].U1)
    {
        std::string key = ds.first;
        unsigned x = ds.second;
        unsigned y = datas[1].U1[key];
        unsigned z = datas[2].U1[key];

        datas[3].U1[key] = x * weight.x + y * weight.y + z * weight.z;
    }

    for (const auto& ds : datas[0].F1)
    {
        std::string key = ds.first;
        float x = ds.second;
        float y = datas[1].F1[key];
        float z = datas[2].F1[key];

        datas[3].F1[key] = x * weight.x + y * weight.y + z * weight.z;
    }

    for (const auto& ds : datas[0].F2)
    {
        std::string key = ds.first;
        Vec2 x = ds.second;
        Vec2 y = datas[1].F2[key];
        Vec2 z = datas[2].F2[key];

        datas[3].F2[key] = x * weight.x + y * weight.y + z * weight.z;
    }

    for (const auto& ds : datas[0].F3)
    {
        std::string key = ds.first;
        Vec3 x = ds.second;
        Vec3 y = datas[1].F3[key];
        Vec3 z = datas[2].F3[key];

        datas[3].F3[key] = x * weight.x + y * weight.y + z * weight.z;
    }

    for (const auto& ds : datas[0].F4)
    {
        std::string key = ds.first;
        Vec4 x = ds.second;
        Vec4 y = datas[1].F4[key];
        Vec4 z = datas[2].F4[key];

        datas[3].F4[key] = x * weight.x + y * weight.y + z * weight.z;
    }
}
