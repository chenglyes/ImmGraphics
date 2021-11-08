#include "immgraphics.h"

using namespace ImmGraphics;

class TestPipeline : public RenderPipeline
{
public:
    TestPipeline(): m_time(0) {}
    virtual ~TestPipeline() = default;

    virtual void StartPipeline(const VertexBuffer &vertices, const IndexBuffer &indices) override
    {

        unsigned i = 0;
        while (i + 3 <= indices.getSize())
        {
            unsigned index0 = indices[i];
            unsigned index1 = indices[i + 1];
            unsigned index2 = indices[i + 2];

            Vec3 v0 = vertices[index0];
            Vec3 v1 = vertices[index1];
            Vec3 v2 = vertices[index2];

            Matrix4 transform = Matrix4::Viewport(m_device->width, m_device->height) * Matrix4::RotateByZAxis(Math::toRad(m_time));
            v0 = transform * Vec4(v0, 1);
            v1 = transform * Vec4(v1, 1);
            v2 = transform * Vec4(v2, 1);

            setLine(v0.x, v0.y, v1.x, v1.y, Color::Cyan);
            setLine(v1.x, v1.y, v2.x, v2.y, Color::Cyan);
            setLine(v2.x, v2.y, v0.x, v0.y, Color::Cyan);

            i += 3;
        }
    }

    void Update() { ++m_time; }

private:
    void setLine(int xf, int yf, int xt, int yt, unsigned color)
    {
        if (xf > xt)
        {
            int t;
            t = xf;
            xf = xt;
            xt = t;
            t = yf;
            yf = yt;
            yt = t;
        }

        int dx = xt - xf;
        int dy = yt - yf;
        int f = 0;

        if (dy >= 0 && dy <= dx)
        {
            for (int x = xf, y = yf; x <= xt; x++)
            {
                setBuffer(x, y, color);
                if ((f + dy) + (f + dy - dx) < 0)
                    f += dy;
                else
                {
                    f += dy - dx;
                    y++;
                }
            }
        }
        else if (dy >= 0 && dy > dx)
        {
            for (int x = xf, y = yf; y <= yt; y++)
            {
                setBuffer(x, y, color);
                if ((f - dx) + (f - dx + dy) > 0)
                    f += -dx;
                else
                {
                    f += -dx + dy;
                    x++;
                }
            }
        }
        else if (dy <= 0 && -dy <= dx)
        {
            for (int x = xf, y = yf; x <= xt; x++)
            {
                setBuffer(x, y, color);
                if ((f + dy) + (f + dy + dx) > 0)
                    f += dy;
                else
                {
                    f += dy + dx;
                    y--;
                }
            }
        }
        else if (dy <= 0 && -dy > dx)
        {
            for (int x = xf, y = yf; y >= yt; y--)
            {
                setBuffer(x, y, color);
                if ((f + dx) + (f + dy + dx) < 0)
                    f += dx;
                else
                {
                    f += dy + dx;
                    x++;
                }
            }
        }
    }

    void setBuffer(unsigned x, unsigned y, unsigned value)
    {
        if (x < m_device->width && y < m_device->height)
            *((unsigned *)m_device->frameBuffer + y * m_device->width + x) = value;
    }

private:
    unsigned m_time;

};

int main()
{
    VertexBuffer vb = {
        { 0, 0.5, 0 },
        { 0.5, -0.5, 0 },
        { -0.5, -0.5, 0 }
    };

    IndexBuffer ib = {
        0, 1, 2
    };

    Window *window = Window::GenerateWindow(806, 829);
    TestPipeline* pipeline = new TestPipeline;

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(pipeline);

    window->Show();

    while (!window->ShouldClose())
    {
        window->ClearBuffer(0x27);
        renderer.Plane();
        renderer.Mesh(vb, ib);
        renderer.Render();
        pipeline->Update();
        window->Draw();
    }

    Window::DestroyWindow(window);
    return 0;
}
