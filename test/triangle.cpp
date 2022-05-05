#include "immgraphics.h"
#include "pipelines/shaderpip.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

void SubmitObjects(Renderer& renderer)
{
    static VertexBuffer vb = {
        { {0, 0.3, 0}, {1, 0, 0} },
        { {0.3, -0.3, 0}, {0, 1, 0} },
        { {-0.3, -0.3, 0}, {0, 0, 1} }
    };

    static IndexBuffer ib = {
        0, 1, 2
    };

    renderer.Mesh(vb, ib);
}

class NormalShader : public Shader
{
public:
    Vec3 VSMain(const Vertex& now, VaryingData& datas) override
    {
        datas.F3["VertexColor"] = now.color;
        return  now.pos;
    }

    Vec3 PSMain(VaryingData& datas) override
    {
        return datas.F3["VertexColor"];
    }

};

int main()
{
    Window* window = Window::GenerateWindow(606, 629);
    ShaderPipeline* pipeline = new ShaderPipeline;
    pipeline->AddShader(new NormalShader);

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(pipeline);

    window->Show();

    SubmitObjects(renderer);
    renderer.Render();
    window->Draw();

    while (!window->ShouldClose())
    {
        using namespace std::chrono;
        std::this_thread::sleep_for(1ms);
    }

    Window::DestroyWindow(window);
    return 0;
}
