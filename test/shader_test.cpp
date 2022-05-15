#include "immgraphics.h"
#include "pipelines/shaderpip.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

void SubmitObjects(Renderer& renderer)
{
    //renderer.Sphere(Vec3(0, 0, 0), 1, 30, Vec3(0.28, 0.35, 0.68));
    //renderer.Plane(Vec3(0, 0.5, 0), Vec3(1, 1, 1), Vec3(1, 0, 0));
    //renderer.Plane(Vec3(0, -0.5, 0), Vec3(1, 1, 1), Vec3(0, 0, 1));
    renderer.Box(Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(1, 0.5, 0.31));
}

class TestShader : public Shader
{
public:
    int time = 10;

    Vec3 cameraPos = Vec3(0, 3, 3);
    Vec3 lightPos = Vec3(0, 4.0, -3.0);
    Vec3 lightColor = Vec3(1, 1, 1);

public:
    Vec3 VSMain(const Vertex& now, VaryingData& datas) override
    {
        Vec4 pos(now.pos, 1);

        //Matrix4 model = Matrix4::RotateByAxis(time * Math::RAD, 65 * Math::RAD, 0);
        Matrix4 model = Matrix4::RotateByXAxis(time * Math::RAD);
        Matrix4 view = Matrix4::View(cameraPos, Vec3::Zero(), Vec3(0, cameraPos.z / cameraPos.Length(), -cameraPos.y / cameraPos.Length()));
        Matrix4 project = Matrix4::Perspective(1.0f, 80 * Math::RAD, 0.1, 100);

        datas.F3["VertexColor"] = now.color;
        datas.F3["FragPos"] = model * pos;
        datas.F3["Normal"] = model.getInversed().getTransposed().getMat3() * now.norm;

        pos = project * view * model * pos;
        pos = pos / pos.w;
        return  pos;
    }

    Vec3 PSMain(VaryingData& datas) override
    {
        Vec3 vertexColor = datas.F3["VertexColor"];
        Vec3 fragPos = datas.F3["FragPos"];
        Vec3 norm = datas.F3["Normal"].getNormalized();

        Vec3 lightDir = (lightPos - fragPos).getNormalized();
        Vec3 viewDir = (cameraPos - fragPos).getNormalized();
        Vec3 reflectDir = (-lightDir).getReflected(norm);

        // ambient
        float ambientStrength = 0.1f;
        Vec3 ambient = lightColor * ambientStrength;

        // diffuse
        float diff = Math::Max(Vec3::Dot(norm, lightDir), 0);
        Vec3 diffuse = lightColor * diff * 0.8;

        // specular
        float specularStrength = 0.8;
        float spec = Math::Pow(Math::Max(Vec3::Dot(viewDir, reflectDir), 0), 256);
        Vec3 specular = lightColor * spec * specularStrength;

        Vec3 effect = ambient + diffuse + specular;
        effect.Clamp01();
        return effect * vertexColor;
    }

};

int main()
{
    Window *window = Window::GenerateWindow(606, 629);
    ShaderPipeline *pipeline = new ShaderPipeline;

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(pipeline);

    window->Show();

    TestShader* phongShader = new TestShader;
    pipeline->SetShader(phongShader);

    SubmitObjects(renderer);

    renderer.Render();
    window->Draw();

    while (!window->ShouldClose())
    {
        // using namespace std::chrono;

        auto fps = CalculateFPS();
        DEBUG_Print("FPS: " << fps);

        ++ phongShader->time;
        window->getDevice()->ClearBuffer();
        renderer.Render();
        window->Draw();

        // std::this_thread::sleep_for(1ms);
    }

    Window::DestroyWindow(window);
    return 0;
}
