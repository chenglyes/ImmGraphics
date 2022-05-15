#include "immgraphics.h"
#include "pipelines/shaderpip.h"
#include "debug.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

void SubmitObjects(Renderer& renderer)
{
    static VertexBuffer vb = {
        { { 0,  0.6, 0}, {1, 0, 0} },
        { { 0.6, -0.6, -0.3}, {0, 1, 0} },
        { {-0.6, -0.6, 0.3}, {0, 0, 1} },
    };

    static IndexBuffer ib = {
        0, 1, 2
    };

    renderer.Mesh(vb, ib);
}

class PhongShader : public Shader
{
public:
    Vec3 VSMain(const Vertex& now, VaryingData& datas) override
    {
        datas.F3["VertexColor"] = now.color;
        return now.pos;
    }

    Vec3 PSMain(VaryingData& datas) override
    {
        return datas.F3["VertexColor"];
    }
};

void RenderProc(Renderer* renderer, RenderDevice* device, ShaderPipeline* pipeline)
{

    //BEGIN_TIMER(First Render)
    //    renderer->Render(device, pipeline);
    //END_TIMER()

    device->ClearBuffer();
    pipeline->SetFastSampled(true);

    BEGIN_TIMER(Render with Fast-Sample 0)
        renderer->Render(device, pipeline);
    END_TIMER()
}

int main()
{
    Renderer renderer;

    Window* window = Window::GenerateWindow(606, 629);
    std::shared_ptr<ShaderPipeline> pipeline(new ShaderPipeline);
    std::shared_ptr<PhongShader> phongShader(new PhongShader);

    pipeline->SetShader(phongShader.get());

    window->Show();

    SubmitObjects(renderer);
    std::thread renderThread(RenderProc, &renderer, window->getDevice(), pipeline.get());

    while (!window->ShouldClose())
    {
        window->Draw();
    }

    renderThread.join();
    Window::DestroyWindow(window);
    return 0;
}
