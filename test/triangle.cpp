#include "immgraphics.h"
#include "pipelines/shaderpip.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

void SubmitObjects(Renderer& renderer)
{
    static VertexBuffer vb = {
        { { 0,  1, 0}, {1, 0, 0} },
        { { 1, -1, 0}, {0, 1, 0} },
        { {-1, -1, 0}, {0, 0, 1} }
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
        Vec4 pos(now.pos, 1);

        Matrix4 model = Matrix4::Identity();
        Matrix4 view = Matrix4::View(Vec3(0, 0, 3), Vec3::Zero(), Vec3(0, 1, 0));
        Matrix4 project = Matrix4::Perspective(1.0f, 100 * Math::RAD, 0.1, 10);

        datas.F3["VertexColor"] = now.color;

        pos = project * view * model * pos;
        pos = pos / pos.w;
        return  pos;
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
