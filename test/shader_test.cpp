#include "immgraphics.h"
#include "pipelines/shaderpip.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

void SubmitObjects(Renderer& renderer)
{
    static VertexBuffer vb = {
        // Front
        {{ -0.2f, -0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{  0.2f, -0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{  0.2f,  0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{  0.2f,  0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{ -0.2f,  0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{ -0.2f, -0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
                                            
        // Back                             
        {{ -0.2f, -0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{  0.2f, -0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{  0.2f,  0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{  0.2f,  0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{ -0.2f,  0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
        {{ -0.2f, -0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },

        // Left
        {{ -0.2f,  0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{ -0.2f,  0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{ -0.2f, -0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{ -0.2f, -0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{ -0.2f, -0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{ -0.2f,  0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
                                                
        // Right                                
        {{  0.2f,  0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{  0.2f,  0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{  0.2f, -0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{  0.2f, -0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{  0.2f, -0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
        {{  0.2f,  0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },

        // Bottom
        {{ -0.2f, -0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{  0.2f, -0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{  0.2f, -0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{  0.2f, -0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{ -0.2f, -0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{ -0.2f, -0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
                                    
        // Top                       
        {{ -0.2f,  0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{  0.2f,  0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{  0.2f,  0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{  0.2f,  0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{ -0.2f,  0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
        {{ -0.2f,  0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } }
    };

    static IndexBuffer ib = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };

    renderer.Mesh(vb, ib);
}

class TestShader : public Shader
{
public:
    int time = 0;

public:
    Vec3 VSMain(const Vertex& now, VaryingData& datas) override
    {
        Vec4 pos(now.pos, 1);
        pos = Matrix4::RotateByXAxis(-time * Math::RAD) * pos;
        pos = Matrix4::RotateByYAxis(-time * Math::RAD) * pos;
        
        // pos = Matrix4::Translate(0, 0, 10) * pos;
        pos = Matrix4::View(Vec3(0, 0, -2), Vec3::Zero(), Vec3(0, 1, 0)) * pos;
        pos = Matrix4::Perspective(1.0f, 160 * Math::RAD, 0.1, 10) * pos;
        pos = pos / pos.w;

        datas.F3["VectexColor"] = now.color;
        return pos;
    }

    Vec3 PSMain(VaryingData& datas) override
    {
        return datas.F3["VectexColor"];
    }

};

int main()
{
    Window *window = Window::GenerateWindow(606, 629);
    ShaderPipeline *pipeline = new ShaderPipeline;

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(pipeline);

    window->Show();

    TestShader* shader = new TestShader;
    pipeline->AddShader(shader);

    SubmitObjects(renderer);

    window->ClearBuffer();
    renderer.Render();
    window->Draw();

    while (!window->ShouldClose())
    {
        // using namespace std::chrono;

        auto fps = CalculateFPS();
        DEBUG_Print("FPS: " << fps);

        ++ shader->time;
        window->ClearBuffer();
        renderer.Render();
        window->Draw();

        // std::this_thread::sleep_for(1ms);
    }

    Window::DestroyWindow(window);
    return 0;
}
