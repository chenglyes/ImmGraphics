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

void Renderer::Triangle(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& color)
{
    Vec3 AC = c - a;
    Vec3 AB = b - a;
    Vec3 normal = Vec3::Cross(AC, AB).getNormalized();

    VertexBuffer vertices = {
        { a, color, normal },
        { b, color, normal },
        { c, color, normal },
    };

    IndexBuffer indices = {
        0, 1, 2
    };

    Mesh(vertices, indices);
}

void Renderer::Plane(const Vec3& pos, const Vec2& size, const Vec3& color)
{
    float left = pos.x - size.x / 2;
    float right = pos.x + size.x / 2;

    float near = pos.z - size.y / 2;
    float far = pos.z + size.y / 2;

    VertexBuffer vertices = {
        { {left,  pos.y, near }, color, Vec3::Up() },
        { {left,  pos.y, far  }, color, Vec3::Up() },
        { {right, pos.y, near }, color, Vec3::Up() },
        { {right, pos.y, far  }, color, Vec3::Up() }
    };

    IndexBuffer indices = {
        0, 1, 2,    // first triangle
        2, 1, 3     // second triangle
    };

    Mesh(vertices, indices);
}

void Renderer::Box(const Vec3& pos, const Vec3& size, const Vec3& color)
{
    //float left = pos.x - size.x / 2;
    //float right = pos.x + size.x / 2;

    //float top = pos.y + size.y / 2;
    //float bottom = pos.y - size.y / 2;

    //float near = pos.z + size.z / 2;
    //float far = pos.z - size.z / 2;

    float hx = size.x / 2;
    float hy = size.y / 2;
    float hz = size.z / 2;

    VertexBuffer vb = {

        // Front
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Forward() },
        { {pos.x + hx, pos.y - hy, pos.z - hz }, color, Vec3::Forward() },
        { {pos.x + hx, pos.y + hy, pos.z - hz }, color, Vec3::Forward() },
        { {pos.x + hx, pos.y + hy, pos.z - hz }, color, Vec3::Forward() },
        { {pos.x - hx, pos.y + hy, pos.z - hz }, color, Vec3::Forward() },
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Forward() },

        // Back
        { {pos.x - hx, pos.y - hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x + hx, pos.y - hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x - hx, pos.y + hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x - hx, pos.y - hy, pos.z + hz }, color, Vec3::Back() },

        // Left
        { {pos.x - hx, pos.y + hy, pos.z + hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y + hy, pos.z - hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y - hy, pos.z + hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y + hy, pos.z + hz }, color, Vec3::Left() },

        // Right
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y + hy, pos.z - hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y - hy, pos.z - hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y - hy, pos.z - hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y - hy, pos.z + hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Right() },

        // Bottom
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Down() },
        { {pos.x + hx, pos.y - hy, pos.z - hz }, color, Vec3::Down() },
        { {pos.x + hx, pos.y - hy, pos.z + hz }, color, Vec3::Down() },
        { {pos.x + hx, pos.y - hy, pos.z + hz }, color, Vec3::Down() },
        { {pos.x - hx, pos.y - hy, pos.z + hz }, color, Vec3::Down() },
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Down() },

        // Top
        { {pos.x - hx, pos.y + hy, pos.z - hz }, color, Vec3::Up() },
        { {pos.x + hx, pos.y + hy, pos.z - hz }, color, Vec3::Up() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Up() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Up() },
        { {pos.x - hx, pos.y + hy, pos.z + hz }, color, Vec3::Up() },
        { {pos.x - hx, pos.y + hy, pos.z - hz }, color, Vec3::Up() }

    };

    //VertexBuffer vb = {
    //    // Front
    //    { { left,  bottom, near }, color, Vec3::Forward() },
    //    { { right, bottom, near }, color, Vec3::Forward() },
    //    { { right,  top,   near }, color, Vec3::Forward() },
    //    { { right,  top,   near }, color, Vec3::Forward() },
    //    { { left,   top,   near }, color, Vec3::Forward() },
    //    { { left,  bottom, near }, color, Vec3::Forward() },

    //    // Back
    //    { { left,  bottom, far  }, color, Vec3::Back() },
    //    { { right, bottom, far  }, color, Vec3::Back() },
    //    { { right,  top,   far  }, color, Vec3::Back() },
    //    { { right,  top,   far  }, color, Vec3::Back() },
    //    { { left,   top,   far  }, color, Vec3::Back() },
    //    { { left,  bottom, far  }, color, Vec3::Back() },

    //    // Left
    //    { { left,   top,   far  }, color, Vec3::Left() },
    //    { { left,   top,   near }, color, Vec3::Left() },
    //    { { left,  bottom, near }, color, Vec3::Left() },
    //    { { left,  bottom, near }, color, Vec3::Left() },
    //    { { left,  bottom, far  }, color, Vec3::Left() },
    //    { { left,   top,   far  }, color, Vec3::Left() },

    //    // Right
    //    { { right,  top,   far  }, color, Vec3::Right() },
    //    { { right,  top,   near }, color, Vec3::Right() },
    //    { { right, bottom, near }, color, Vec3::Right() },
    //    { { right, bottom, near }, color, Vec3::Right() },
    //    { { right, bottom, far  }, color, Vec3::Right() },
    //    { { right,  top,   far  }, color, Vec3::Right() },

    //    // Bottom
    //    { { left,  bottom, near }, color, Vec3::Down() },
    //    { { right, bottom, near }, color, Vec3::Down() },
    //    { { right, bottom, far  }, color, Vec3::Down() },
    //    { { right, bottom, far  }, color, Vec3::Down() },
    //    { { left,  bottom, far  }, color, Vec3::Down() },
    //    { { left,  bottom, near }, color, Vec3::Down() },

    //    // Top
    //    { { left,   top,   near }, color, Vec3::Up() },
    //    { { right,  top,   near }, color, Vec3::Up() },
    //    { { right,  top,   far  }, color, Vec3::Up() },
    //    { { right,  top,   far  }, color, Vec3::Up() },
    //    { { left,   top,   far  }, color, Vec3::Up() },
    //    { { left,   top,   near }, color, Vec3::Up() }
    //};

    IndexBuffer ib;
    for (int i = 0; i < 36; ++i) ib.PushBack(i);

    Mesh(vb, ib);
}

void Renderer::Sphere(const Vec3& pos, float radius, int slices, const Vec3& color)
{
    if (slices < 0) return;
    if ((slices & 1) == 1) ++slices;

    float delta = 2 * Math::PI / slices;
    Vertex v;
    float deltaU = 1.0f / slices;
    float deltaV = deltaU * 2;
    int layerNum = slices / 2 + 1;
    int perLayerNum = slices + 1;

    VertexBuffer vb(layerNum * perLayerNum);

    v.color = color;

    // 层数
    for (int i = 0; i < layerNum; i++) {
        // 每层的高度(即pointY)，为负数让其从下向上创建
        v.pos.y = -radius * Math::Cos(delta * i);

        // 每层的半径
        float layerRadius = radius * sin(delta * i);
        // 每层圆的点,
        for (int j = 0; j < perLayerNum; j++) {
            // 计算
            v.pos.x = layerRadius * Math::Cos(delta * j);
            v.pos.z = layerRadius * Math::Sin(delta * j);
            v.uv.x = deltaU * j;
            v.uv.y = deltaV * i;

            v.norm = v.pos.getNormalized();
            v.pos = v.pos + pos;

            vb[i * perLayerNum + j] = v;
        }
    }

    IndexBuffer ib((slices + 1) * (slices + 1));
    layerNum = slices / 2 + 1;
    perLayerNum = slices + 1;

    for (int i = 0; i < layerNum; i++) {

        if (i + 1 < layerNum) {

            for (int j = 0; j < perLayerNum; j++) {

                // i * perLayerNum * 2每层的下标是原来的2倍
                ib[(i * perLayerNum * 2) + (j * 2)] = i * perLayerNum + j;
                // 后一层数据
                ib[(i * perLayerNum * 2) + (j * 2 + 1)] = (i + 1) * perLayerNum + j;
            }
        }
        else {

            for (int j = 0; j < perLayerNum; j++) {
                // 后最一层数据单独处理
                ib[i * perLayerNum * 2 + j] = i * perLayerNum + j;
            }
        }
    }

    Mesh(vb, ib);
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
