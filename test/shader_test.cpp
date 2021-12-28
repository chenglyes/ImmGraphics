#include "immgraphics.h"
#include "pipelines/shaderpip.h"

#include <cstdlib>

using namespace ImmGraphics;

class TestShader : public Shader
{
public:
    TestShader()
    {

    }

    virtual ~TestShader()
    {

    }

public:
    virtual void VSMain(Vertex& now)
    {

    }

    virtual unsigned PSMain()
    {
        return 0x1E1F1C;
    }

};

int main()
{
    VertexBuffer vb = {
        {{0, 0.5, 0}, Color::Red},
        {{0.5, -0.5, 0}, Color::Green},
        {{-0.5, -0.5, 0}, Color::Blue}
    };

    IndexBuffer ib = {
        0, 1, 2
    };

    Window *window = Window::GenerateWindow(806, 829);
    ShaderPipeline *pipeline = new ShaderPipeline;

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(pipeline);

    window->Show();

    pipeline->AddShader(new TestShader);

    while (!window->ShouldClose())
    {
        auto fps = CalculateFPS();
        DEBUG_Print("FPS: " << fps);
        // window->ClearBuffer(0x1E1F1C);
        renderer.Mesh(vb, ib);
        renderer.Plane(Color::White);
        renderer.Render();
        window->Draw();
    }

    Window::DestroyWindow(window);
    return 0;
}
