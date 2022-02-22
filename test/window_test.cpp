#include "immgraphics.h"

#include <cstdlib>
#include <chrono>
#include <thread>
#include <omp.h>

using namespace ImmGraphics;

class TestPipeline : public RenderPipeline
{
public:
    TestPipeline() {}
    virtual ~TestPipeline() = default;

    virtual void StartPipeline(const VertexBuffer &vertices, const IndexBuffer &indices) override
    {
        #pragma omp parallel for
        for (int i = 0; i < indices.getSize(); i += 3)
        {
            unsigned index0 = indices[i];
            unsigned index1 = indices[i + 1];
            unsigned index2 = indices[i + 2];

            Vertex v0 = vertices[index0];
            Vertex v1 = vertices[index1];
            Vertex v2 = vertices[index2];

            Matrix4 transform = Matrix4::Viewport(m_device->width, m_device->height);
            v0.pos = transform * Vec4(v0.pos, 1);
            v1.pos = transform * Vec4(v1.pos, 1);
            v2.pos = transform * Vec4(v2.pos, 1);

            setLine(v0.pos.x, v0.pos.y, v1.pos.x, v1.pos.y, v0.color);
            setLine(v1.pos.x, v1.pos.y, v2.pos.x, v2.pos.y, v1.color);
            setLine(v2.pos.x, v2.pos.y, v0.pos.x, v0.pos.y, v2.color);

        }
    }

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

};

void RenderObjects(Renderer& renderer)
{
    static VertexBuffer vb = {
        { {0, 0.5, 0}, Color::Red },
        { {0.5, -0.5, 0}, Color::Green },
        { {-0.5, -0.5, 0}, Color::Blue }
    };

    static IndexBuffer ib = {
        0, 1, 2
    };

    renderer.Mesh(vb, ib);
    renderer.Plane(Color::White);
    renderer.Render();
}

int main()
{
    // Create window.
    Window *window = Window::GenerateWindow(806, 829);

    // Create renderer.
    Renderer renderer(window->getDevice());

    // Set pipelines.
    renderer.AddPipeline(new TestPipeline);

    // Render objects.
    RenderObjects(renderer);

    // Copy the buffer to window.
    // window->Draw();

    // Show the window.
    window->Show();

    while (!window->ShouldClose())
    {
        using namespace std::chrono;
        std::this_thread::sleep_for(1ms);
    }

    // Destroy the window.
    Window::DestroyWindow(window);
    return 0;
}
