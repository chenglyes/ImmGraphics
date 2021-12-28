#include "immgraphics.h"

#include <cstdlib>

using namespace ImmGraphics;

class TestPipeline : public RenderPipeline
{
public:
    TestPipeline(): m_rad(0) {}
    virtual ~TestPipeline() = default;

    virtual void StartPipeline(const VertexBuffer &vertices, const IndexBuffer &indices) override
    {

        #pragma omp parallel for
        for (unsigned i = 0; i < indices.getSize(); i += 3)
        {
            unsigned index0 = indices[i];
            unsigned index1 = indices[i + 1];
            unsigned index2 = indices[i + 2];

            Vertex v0 = vertices[index0];
            Vertex v1 = vertices[index1];
            Vertex v2 = vertices[index2];

            Matrix4 transform = Matrix4::Viewport(m_device->width, m_device->height) * Matrix4::RotateByZAxis(Math::toRad(m_rad));
            v0.pos = transform * Vec4(v0.pos, 1);
            v1.pos = transform * Vec4(v1.pos, 1);
            v2.pos = transform * Vec4(v2.pos, 1);

            setLine(v0.pos.x, v0.pos.y, v1.pos.x, v1.pos.y, v0.color);
            setLine(v1.pos.x, v1.pos.y, v2.pos.x, v2.pos.y, v1.color);
            setLine(v2.pos.x, v2.pos.y, v0.pos.x, v0.pos.y, v2.color);

        }
    } 

    void Update(float delta) { m_rad += delta * 8; }

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
    float m_rad;

};

int main()
{
    VertexBuffer vb = {
        { {0, 0.5, 0}, Color::Red },
        { {0.5, -0.5, 0}, Color::Green },
        { {-0.5, -0.5, 0}, Color::Blue }
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
        auto fps = CalculateFPS();
        DEBUG_Print("FPS: " << fps);
        window->ClearBuffer(0x1E1F1C);
        renderer.Mesh(vb, ib);
        renderer.Plane(Color::White);
        renderer.Render();
        pipeline->Update(getDeltaTime());
        window->Draw();
    }

    Window::DestroyWindow(window);
    return 0;
}
