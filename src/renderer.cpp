#include "renderer.h"

using namespace ImmGraphics;

Renderer::Renderer()
{
    
    
}

Renderer::~Renderer()
{
    
}

void Renderer::Render(RenderDevice* target, RenderPipeline* pipeline)
{
    _DB_ASSERT(target && "No avaliable render target.");
    _DB_ASSERT(pipeline && "Invalid render pipeline.");

    pipeline->setRenderTarget(target);
    pipeline->StartPipeline(m_vertices, m_indices);
}

void Renderer::Clear()
{
    m_vertices.Clear();
    m_indices.Clear();
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

    float near = pos.z + size.y / 2;
    float far = pos.z - size.y / 2;

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
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x + hx, pos.y - hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x - hx, pos.y - hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x - hx, pos.y + hy, pos.z + hz }, color, Vec3::Back() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Back() },
        
        // Left
        { {pos.x - hx, pos.y + hy, pos.z + hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y + hy, pos.z - hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y + hy, pos.z + hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y - hy, pos.z + hz }, color, Vec3::Left() },
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Left() },

        // Right
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y + hy, pos.z - hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y - hy, pos.z - hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y - hy, pos.z - hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y - hy, pos.z + hz }, color, Vec3::Right() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Right() },

        // Bottom
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Down() },
        { {pos.x + hx, pos.y - hy, pos.z + hz }, color, Vec3::Down() },
        { {pos.x + hx, pos.y - hy, pos.z - hz }, color, Vec3::Down() },
        { {pos.x - hx, pos.y - hy, pos.z - hz }, color, Vec3::Down() },
        { {pos.x - hx, pos.y - hy, pos.z + hz }, color, Vec3::Down() },
        { {pos.x + hx, pos.y - hy, pos.z + hz }, color, Vec3::Down() },
        

        // Top
        { {pos.x - hx, pos.y + hy, pos.z - hz }, color, Vec3::Up() },
        { {pos.x + hx, pos.y + hy, pos.z - hz }, color, Vec3::Up() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Up() },
        { {pos.x - hx, pos.y + hy, pos.z - hz }, color, Vec3::Up() },
        { {pos.x + hx, pos.y + hy, pos.z + hz }, color, Vec3::Up() },
        { {pos.x - hx, pos.y + hy, pos.z + hz }, color, Vec3::Up() },

    };

    IndexBuffer ib;
    for (int i = 0; i < 36; ++i) ib.PushBack(i);

    Mesh(vb, ib);
}

void Renderer::Sphere(const Vec3& pos, float radius, int slices, const Vec3& color)
{
    VertexBuffer vb;
    IndexBuffer ib;

    int stacks = slices;

    Vertex v;
    v.color = color;
    
    // add top vertex
    v.pos = Vec3(0, 0.5, 0);
    v.norm = Vec3::Up();
    int id_top = vb.getSize();
    vb.PushBack(v);

    // generate vertices per stack / slice
    for (int i = 0; i < stacks - 1; i++)
    {
        auto phi = Math::PI * float(i + 1) / float(stacks);
        for (int j = 0; j < slices; j++)
        {
            float theta = 2 * Math::PI * float(j) / float(slices);
            float x = std::sin(phi) * std::cos(theta) / 2;
            float y = std::cos(phi) / 2;
            float z = std::sin(phi) * std::sin(theta) / 2;
            v.pos = Vec3(x, y, z);
            v.norm = v.pos.getNormalized();
            vb.PushBack(v);
        }
    }

    // add bottom vertex
    v.pos = Vec3(0, -0.5, 0);
    v.norm = Vec3::Down();
    float id_bottom = vb.getSize();
    vb.PushBack(v);

    // add top / bottom triangles
    for (int i = 0; i < slices; ++i)
    {
        int i0 = i + 1;
        int i1 = (i + 1) % slices + 1;
        ib.PushBack(id_top);
        ib.PushBack(i0);
        ib.PushBack(i1);

        i0 = i + slices * (stacks - 2) + 1;
        i1 = (i + 1) % slices + slices * (stacks - 2) + 1;
        ib.PushBack(id_bottom);
        ib.PushBack(i1);
        ib.PushBack(i0);
    }

    // add quads per stack / slice
    for (int j = 0; j < stacks - 2; j++)
    {
        int j0 = j * slices + 1;
        int j1 = (j + 1) * slices + 1;
        for (int i = 0; i < slices; i++)
        {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % slices;
            auto i2 = j1 + (i + 1) % slices;
            auto i3 = j1 + i;

            ib.PushBack(i0);
            ib.PushBack(i3);
            ib.PushBack(i2);

            ib.PushBack(i0);
            ib.PushBack(i2);
            ib.PushBack(i1);
        }
    }

    Mesh(vb, ib);
}

void Renderer::Tetrahedron(const Vec3& color)
{
    constexpr float sqr3 = 1.73205f;
    constexpr float sqr3d3 = sqr3 / 3;
    constexpr float sqr3d6 = sqr3 / 6;

    Vec3 v0(0, 0, sqr3d3);
    Vec3 v1(0.5, 0, -sqr3d6);
    Vec3 v2(-0.5, 0, -sqr3d6);
    Vec3 v3(0, 2.0f / 3.0f, 0);

    Vec3 n0 = Vec3::Down();
    Vec3 n1 = Vec3::Cross(v3 - v0, v2 - v0).getNormalized();
    Vec3 n2 = Vec3::Cross(v1 - v0, v3 - v0).getNormalized();
    Vec3 n3 = Vec3::Cross(v2 - v1, v3 - v1).getNormalized();

    VertexBuffer vb = {
        { v0, color, n0 },
        { v1, color, n0 },
        { v2, color, n0 },

        { v0, color, n1 },
        { v2, color, n1 },
        { v3, color, n1 },

        { v0, color, n2 },
        { v3, color, n2 },
        { v1, color, n2 },

        { v1, color, n3 },
        { v3, color, n3 },
        { v2, color, n3 }
    };

    IndexBuffer ib = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

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
