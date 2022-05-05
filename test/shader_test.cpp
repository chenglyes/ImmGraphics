#include "immgraphics.h"
#include "pipelines/shaderpip.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

void SubmitObjects(Renderer& renderer)
{
    //static VertexBuffer vb = {
    //    // Front
    //    {{ -0.2f, -0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{  0.2f, -0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{  0.2f,  0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{  0.2f,  0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{ -0.2f,  0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{ -0.2f, -0.2f, -0.2f }, { 0.8f, 0.1f, 0.2f } },
    //                                        
    //    // Back                             
    //    {{ -0.2f, -0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{  0.2f, -0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{  0.2f,  0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{  0.2f,  0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{ -0.2f,  0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },
    //    {{ -0.2f, -0.2f,  0.2f }, { 0.8f, 0.1f, 0.2f } },

    //    // Left
    //    {{ -0.2f,  0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{ -0.2f,  0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{ -0.2f, -0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{ -0.2f, -0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{ -0.2f, -0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{ -0.2f,  0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
    //                                            
    //    // Right                                
    //    {{  0.2f,  0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{  0.2f,  0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{  0.2f, -0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{  0.2f, -0.2f, -0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{  0.2f, -0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },
    //    {{  0.2f,  0.2f,  0.2f }, { 0.2f, 0.8f, 0.1f } },

    //    // Bottom
    //    {{ -0.2f, -0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{  0.2f, -0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{  0.2f, -0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{  0.2f, -0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{ -0.2f, -0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{ -0.2f, -0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
    //                                
    //    // Top                       
    //    {{ -0.2f,  0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{  0.2f,  0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{  0.2f,  0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{  0.2f,  0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{ -0.2f,  0.2f,  0.2f }, { 0.1f, 0.2f, 0.8f } },
    //    {{ -0.2f,  0.2f, -0.2f }, { 0.1f, 0.2f, 0.8f } }
    //};

    //static IndexBuffer ib = {
    //    0, 1, 2,
    //    3, 4, 5,
    //    6, 7, 8,
    //    9, 10, 11,
    //    12, 13, 14,
    //    15, 16, 17,
    //    18, 19, 20,
    //    21, 22, 23,
    //    24, 25, 26,
    //    27, 28, 29,
    //    30, 31, 32,
    //    33, 34, 35
    //};

    //renderer.Mesh(vb, ib);
    //renderer.Box(Vec3(-0.2, 0, 0), Vec3(0.3, 0.3, 0.3), Vec3(0.28, 0.35, 0.68));
    //renderer.Box(Vec3(0.2, 0, 0), Vec3(0.15, 0.15, 0.15), Vec3(0.68, 0.35, 0.48));
    renderer.Sphere(Vec3(0, 0, 0), 0.2, 32, Vec3(1, 0, 0));
    //renderer.Plane(Vec3(0, 0.2, 0), Vec3(2, 2), Vec3(1, 1, 1));
}

class TestShader : public Shader
{
public:
    int time = 0;

    Vec3 viewPos = Vec3(0, 0, -3);
    Vec3 lightPos = Vec3(-0.4, -0.2, -0.4);
    Vec3 lightColor = Vec3(1, 1, 1);

public:
    Vec3 VSMain(const Vertex& now, VaryingData& datas) override
    {
        Vec4 pos(now.pos, 1);

        Matrix4 model = Matrix4::RotateByAxis(-time * Math::RAD, -time * Math::RAD, 0);
        Matrix4 view = Matrix4::View(Vec3(0, 0, -3), Vec3::Zero(), Vec3(0, 1, 0));
        Matrix4 project = Matrix4::Perspective(1.0f, 120 * Math::RAD, 0.1, 10);

        datas.F3["VectexColor"] = now.color;
        datas.F3["FragPos"] = model * pos;
        datas.F3["Normal"] = model * Vec4(now.norm, 1);

        pos = project * view * model * pos;
        pos = pos / pos.w;
        return  pos;
    }

    Vec3 PSMain(VaryingData& datas) override
    {
        Vec3 vertexColor = datas.F3["VectexColor"];
        Vec3 fragPos = datas.F3["FragPos"];
        Vec3 norm = datas.F3["Normal"].getNormalized();

        Vec3 lightDir = (lightPos - fragPos).getNormalized();
        Vec3 viewDir = (viewPos - fragPos).getNormalized();
        Vec3 reflectDir = (-lightDir).getReflected(norm);

        // ambient
        float ambientStrength = 0.2f;
        Vec3 ambient = lightColor * ambientStrength;

        // diffuse
        float diff = Math::Max(norm * lightDir, 0);
        Vec3 diffuse = lightColor * diff;

        // specular
        float specularStrength = 0.65f;
        float spec = Math::Pow(Math::Max(viewDir * reflectDir, 0), 64);
        Vec3 specular = lightColor * spec * specularStrength;

        Vec3 effect = ambient + diffuse + specular;

        Vec3 fragColor;
        fragColor.x = vertexColor.x * effect.x;
        fragColor.y = vertexColor.y * effect.y;
        fragColor.z = vertexColor.z * effect.z;

        return fragColor;
    }

};

int main()
{
    Window *window = Window::GenerateWindow(406, 429);
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
