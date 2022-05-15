#include "pipelines/shaderpip.h"

#include <queue>

using namespace ImmGraphics;

ShaderPipeline::ShaderPipeline()
    :m_shader(nullptr), m_backCulled(false), m_mutiSampled(false), m_fastSampled(false)
{

}

ShaderPipeline::~ShaderPipeline()
{
    
}

void ShaderPipeline::StartPipeline(const VertexBuffer &vertices, const IndexBuffer &indices)
{
    // #pragma omp parallel for
    for (int i = 0; i < indices.getSize(); i += 3)
    {
        if (i + 3 > indices.getSize()) break;

        // render one triangle
        RenderPrimitive(vertices, indices[i], indices[i + 1], indices[i + 2]);
    }
}

void ShaderPipeline::SetShader(Shader* shader)
{
    _DB_ASSERT(shader && "Invalid shader.");
    m_shader = shader;
}

void ShaderPipeline::SetBackCulled(bool option)
{
    m_backCulled = option;
}

void ShaderPipeline::SetMutiSampled(bool option)
{
    m_mutiSampled = option;
}

void ShaderPipeline::SetFastSampled(bool option)
{
    m_fastSampled = option;
}

Vec3 ShaderPipeline::BufferToRelativeTView(const Vec3& position)
{
    Vec3 res;
    res.x = position.x / m_device->GetWidth() - 0.5;
    res.y = position.y / m_device->GetHeight() - 0.5;
    res.z = position.z;
    return res;
}

Vec3 ShaderPipeline::RelativeToBufferView(const Vec3& position)
{
    Vec3 res;
    res.x = (position.x + 1) * m_device->GetWidth() / 2;
    res.y = (position.y + 1) * m_device->GetHeight() / 2;
    res.z = position.z;
    return res;
    //return Matrix4::Viewport(m_device->GetWidth(), m_device->GetHeight()) * Vec4(res, 1);
}

void ShaderPipeline::RenderPrimitive(const VertexBuffer& vertices, int id_a, int id_b, int id_c)
{
    VaryingData datas[4] = {};

    // vertex shader
    Vec3 a = RelativeToBufferView(m_shader->VSMain(vertices[id_a], datas[0]));
    Vec3 b = RelativeToBufferView(m_shader->VSMain(vertices[id_b], datas[1]));
    Vec3 c = RelativeToBufferView(m_shader->VSMain(vertices[id_c], datas[2]));

    if (m_backCulled)
    {
        Vec2 ba = a.xy() - b.xy();
        Vec2 bc = c.xy() - b.xy();
        if (Vec2::Cross(ba, bc) <= 0) return;
    }

    Triangle3D triangle(a, b, c);

    if (m_fastSampled) FastRasterize(triangle, datas);
    else Rasterize(triangle, datas);
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

void ShaderPipeline::Rasterize(const Triangle3D& triangle, VaryingData* datas)
{
    // rasterize
    float lx = Math::Min(triangle.a.x, triangle.b.x, triangle.c.x);
    float rx = Math::Max(triangle.a.x, triangle.b.x, triangle.c.x);
    float ty = Math::Min(triangle.a.y, triangle.b.y, triangle.c.y);
    float by = Math::Max(triangle.a.y, triangle.b.y, triangle.c.y);

    int left = lx;
    int top = ty;
    int right = rx;
    int bottom = by;

    if (!Math::NearZero(rx - right)) ++right;
    if (!Math::NearZero(by - bottom)) ++bottom;

    int width = m_device->GetWidth();
    int height = m_device->GetHeight();

    if (left < 0) left = 0;
    if (top < 0) top = 0;
    if (right >= width) right = width - 1;
    if (bottom >= height) bottom = height - 1;

    RasterizeRect(triangle, datas, RectI(left, top, right, bottom));
}

void ShaderPipeline::RasterizeRect(const Triangle3D& triangle, VaryingData* datas, const RectI& rect)
{
    Triangle2D t = triangle.Project();

    for (int x = rect.left; x <= rect.right; ++x)
    {
        for (int y = rect.top; y <= rect.bottom; ++y)
        {
            Vec2 p(x + 0.5, y + 0.5);
            if (!t.Contain(p)) continue;

            // Handle varying datas
            Vec3 weight = t.Interpolation(p);
            HandleVarying(weight, datas);

            // pixel shader
            Vec3 color = m_shader->PSMain(datas[3]);

            float z = triangle.a.z * weight.x + triangle.b.z * weight.y + triangle.c.z * weight.z;
            float dp = m_device->GetZ(x, y);
            if (z < dp)
            {
                m_device->SetPixel(x, y, color);
                m_device->SetZ(x, y, z);
            }
        }
    }
}

void ShaderPipeline::FastRasterize(const Triangle3D& triangle, VaryingData* datas)
{
    float lx = Math::Min(triangle.a.x, triangle.b.x, triangle.c.x);
    float rx = Math::Max(triangle.a.x, triangle.b.x, triangle.c.x);
    float ty = Math::Min(triangle.a.y, triangle.b.y, triangle.c.y);
    float by = Math::Max(triangle.a.y, triangle.b.y, triangle.c.y);

    int left = lx;
    int top = ty;
    int right = rx;
    int bottom = by;

    if (!Math::NearZero(rx - right)) ++right;
    if (!Math::NearZero(by - bottom)) ++bottom;

    int width = m_device->GetWidth();
    int height = m_device->GetHeight();

    if (left < 0) left = 0;
    if (top < 0) top = 0;
    if (right >= width) right = width - 1;
    if (bottom >= height) bottom = height - 1;

    Triangle2D t = triangle.Project();

    std::queue<RectI> list;

    RectI rect(left, top, right, bottom);
    list.push(rect);
    while (!list.empty())
    {
        rect = list.front();
        list.pop();

        if (rect.width() > 49 && rect.height() > 49)
        {
            Vec2 a, b, c, d;
            int mx = (rect.left + rect.right) >> 1;
            int my = (rect.top + rect.bottom) >> 1;

            RectI rect1(rect.left, rect.top, mx, my);
            a = rect1.lt() + Vec2( 1.5f,  1.5f);
            b = rect1.rt() + Vec2(-1.0f,  1.5f);
            c = rect1.lb() + Vec2( 1.5f, -1.0f);
            d = rect1.rb() + Vec2(-1.0f, -1.0f);
            if (t.Contain(a) || t.Contain(b) || t.Contain(c) || t.Contain(d))
                list.push(rect1);

            RectI rect2(mx + 1, rect.top, rect.right, my);
            a = rect2.lt() + Vec2(1.5f, 1.5f);
            b = rect2.rt() + Vec2(-1.0f, 1.5f);
            c = rect2.lb() + Vec2(1.5f, -1.0f);
            d = rect2.rb() + Vec2(-1.0f, -1.0f);
            if (t.Contain(a) || t.Contain(b) || t.Contain(c) || t.Contain(d))
                list.push(rect2);

            RectI rect3(rect.left, my + 1, mx, rect.bottom);
            a = rect3.lt() + Vec2(1.5f, 1.5f);
            b = rect3.rt() + Vec2(-1.0f, 1.5f);
            c = rect3.lb() + Vec2(1.5f, -1.0f);
            d = rect3.rb() + Vec2(-1.0f, -1.0f);
            if (t.Contain(a) || t.Contain(b) || t.Contain(c) || t.Contain(d))
                list.push(rect3);

            RectI rect4(mx + 1, my + 1, rect.right, rect.bottom);
            a = rect4.lt() + Vec2(1.5f, 1.5f);
            b = rect4.rt() + Vec2(-1.0f, 1.5f);
            c = rect4.lb() + Vec2(1.5f, -1.0f);
            d = rect4.rb() + Vec2(-1.0f, -1.0f);
            if (t.Contain(a) || t.Contain(b) || t.Contain(c) || t.Contain(d))
                list.push(rect4);
        }
        else
        {
            for (int x = rect.left; x <= rect.right; ++x)
            {
                for (int y = rect.top; y <= rect.bottom; ++y)
                {
                    Vec2 p(x + 0.5, y + 0.5);
                    if (!t.Contain(p)) continue;

                    // Handle varying datas
                    Vec3 weight = t.Interpolation(p);
                    HandleVarying(weight, datas);

                    // pixel shader
                    Vec3 color = m_shader->PSMain(datas[3]);

                    float z = triangle.a.z * weight.x + triangle.b.z * weight.y + triangle.c.z * weight.z;
                    float dp = m_device->GetZ(x, y);
                    if (z < dp)
                    {
                        m_device->SetPixel(x, y, color);
                        m_device->SetZ(x, y, z);
                    }
                }
            }
        }
    }


    //RasterizeRect(triangle, datas, rect);
}
