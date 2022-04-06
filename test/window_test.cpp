#include "immgraphics.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

class TestPipeline : public RenderPipeline
{
public:
    TestPipeline() {}
    virtual ~TestPipeline() = default;

    virtual void StartPipeline(const VertexBuffer &vertices, const IndexBuffer &indices) override
    {
        #pragma omp parallel for
        for (int i = 0; i < (int)indices.getSize(); i += 3)
        {
            size_t index0 = indices[static_cast<size_t>(i)];
            size_t index1 = indices[static_cast<size_t>(i) + 1];
            size_t index2 = indices[static_cast<size_t>(i) + 2];

            Vertex v0 = vertices[index0];
            Vertex v1 = vertices[index1];
            Vertex v2 = vertices[index2];

            Matrix4 transform = Matrix4::Viewport((float)m_device->width, (float)m_device->height);
            v0.pos = transform * Vec4(v0.pos, 1);
            v1.pos = transform * Vec4(v1.pos, 1);
            v2.pos = transform * Vec4(v2.pos, 1);

            setLine((int)v0.pos.x, (int)v0.pos.y, (int)v1.pos.x, (int)v1.pos.y, v0.color);
            setLine((int)v1.pos.x, (int)v1.pos.y, (int)v2.pos.x, (int)v2.pos.y, v1.color);
            setLine((int)v2.pos.x, (int)v2.pos.y, (int)v0.pos.x, (int)v0.pos.y, v2.color);

        }
    }

private:
    void setLine(int xf, int yf, int xt, int yt, const Vec3& color)
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
                setBuffer(x, y, 0, Color(color.x, color.y, color.z).getRGBValue());
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
                setBuffer(x, y, 0, Color(color.x, color.y, color.z).getRGBValue());
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
                setBuffer(x, y, 0, Color(color.x, color.y, color.z).getRGBValue());
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
                setBuffer(x, y, 0, Color(color.x, color.y, color.z).getRGBValue());
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

// All objects rendered in here.
void SubmitObjects(Renderer& renderer)
{
    static VertexBuffer vb = {
        { {0, 0.5, 0}, {1, 0, 0} },
        { {0.5, -0.5, 0}, {0, 1, 0} },
        { {-0.5, -0.5, 0}, {0, 0, 1} }
    };

    static IndexBuffer ib = {
        0, 1, 2
    };

    renderer.Mesh(vb, ib);
    renderer.Plane(Vec3(1, 1, 1));
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
    SubmitObjects(renderer);

    // Copy the buffer to window.
    // window->Draw();

    // Show the window.
    window->Show();

    while (!window->ShouldClose())
    {
        using namespace std::chrono;

        auto fps = CalculateFPS();
        DEBUG_Print("FPS: " << fps);

        window->ClearBuffer();
        renderer.Render();
        window->Draw();

        // std::this_thread::sleep_for(1ms);
    }

    // Destroy the window.
    Window::DestroyWindow(window);
    return 0;
}
