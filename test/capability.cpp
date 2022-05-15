#include "immgraphics.h"
#include "pipelines/shaderpip.h"
#include "debug.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

void SubmitObjects(Renderer& renderer)
{
    renderer.Plane(Vec3::Zero(), Vec2(2, 2), Vec3(1, 0.5, 0.31));
}

class PhongShader : public Shader
{
    Vec3 cameraPos = Vec3(0, 3, 3);
public:
    Vec3 VSMain(const Vertex& now, VaryingData& datas) override
    {
        Vec4 pos(now.pos, 1);
        datas.F3["VertexColor"] = now.color;

        Matrix4 model = Matrix4::RotateByXAxis(90 * Math::RAD);

        datas.F3["VertexColor"] = now.color;

        pos = model * pos;
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
    Window* window = Window::GenerateWindow(606, 329);
    ShaderPipeline* pipeline = new ShaderPipeline;
    pipeline->SetShader(new PhongShader);

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(pipeline);

    window->Show();

    SubmitObjects(renderer);

    BEGIN_TIMER(Render plane)
        renderer.Render();
    END_TIMER()

        window->Draw();

    while (!window->ShouldClose())
    {
        using namespace std::chrono;
        std::this_thread::sleep_for(1ms);
    }

    Window::DestroyWindow(window);
    return 0;
}
