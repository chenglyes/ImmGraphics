#include "immgraphics.h"
#include "pipelines/shaderpip.h"

#include <cstdlib>
#include <chrono>
#include <thread>

using namespace ImmGraphics;

class ColorShader : public Shader
{
public:
    Matrix4 model;
    Matrix4 view;
    Matrix4 project;

public:
    Vec3 VSMain(const Vertex& now, VaryingData& datas) override
    {
        Vec4 pos(now.pos, 1);

        pos = project * view * model * pos;
        pos = pos / pos.w;
        return  pos.xyz();
    }

    Vec3 PSMain(VaryingData& datas) override
    {
        return Vec3(1, 1, 1);
    }
};

class PhongShader : public Shader
{
public:
    struct Material
    {
        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;
        float shininess;
    };

    struct PointLight
    {
        Vec3 position;

        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };


    Vec3 cameraPos;
    Material material;
    PointLight pointLight;

    Matrix4 model;
    Matrix4 view;
    Matrix4 project;

    Vec3 VSMain(const Vertex& now, VaryingData& datas) override
    {
        Vec4 pos(now.pos, 1);

        datas.F3["FragPos"] = model * pos;
        datas.F3["Normal"] = model.getInversed().getTransposed().getMat3() * now.norm;

        pos = project * view * model * pos;
        pos = pos / pos.w;
        return pos.xyz();
    }

    Vec3 PSMain(VaryingData& datas) override
    {
        Vec3 vertexColor = datas.F3["VectexColor"];
        Vec3 fragPos = datas.F3["FragPos"];
        Vec3 norm = datas.F3["Normal"].getNormalized();

        // ambient
        Vec3 ambient = pointLight.ambient * material.ambient;

        // diffuse
        Vec3 lightDir = (pointLight.position - fragPos).getNormalized();
        float diff = Math::Max(Vec3::Dot(norm, lightDir), 0);
        Vec3 diffuse = pointLight.diffuse * (material.diffuse * diff);

        // specular
        Vec3 viewDir = (cameraPos - fragPos).getNormalized();
        Vec3 reflectDir = (-lightDir).getReflected(norm);
        float spec = Math::Pow(Math::Max(Vec3::Dot(viewDir, reflectDir), 0), material.shininess);
        Vec3 specular = pointLight.specular * (material.specular * spec);

        float distance = Vec3::Distance(pointLight.position, fragPos);
        float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance +
            pointLight.quadratic * (distance * distance));

        ambient = ambient * attenuation;
        diffuse = diffuse * attenuation;
        specular = specular * attenuation;

        Vec3 result = ambient + diffuse + specular;
        result.Clamp01();
        return result;
    }

};

Window* window = nullptr;
ShaderPipeline* pipeline = nullptr;
ColorShader* colorShader = nullptr;
PhongShader* phongShader = nullptr;
Renderer renderer;

Vec3 pointLightPos = Vec3(1.0f, 0.5f, 1.0f);
Vec3 cameraPos = Vec3(0, 0, 6);
Matrix4 viewMat = Matrix4::View(cameraPos, Vec3::Zero(), Vec3::Up());
Matrix4 projectMat = Matrix4::Perspective(1.0f, 45 * Math::RAD, 0.1, 100);

void RenderProc()
{
    colorShader->view = viewMat;
    colorShader->project = projectMat;

    phongShader->cameraPos = cameraPos;
    phongShader->view = viewMat;
    phongShader->project = projectMat;

    pipeline->SetShader(colorShader);

    // Light
    renderer.Box(Vec3::Zero(), Vec3::Identity(), {});
    colorShader->model = Matrix4::Translate(pointLightPos) * Matrix4::Scale(0.2, 0.2, 0.2);
    renderer.Render(window->getDevice(), pipeline);
    renderer.Clear();

    phongShader->pointLight.position = pointLightPos;
    phongShader->pointLight.specular = Vec3(0.8f, 0.8f, 0.8f);
    phongShader->pointLight.ambient = Vec3(0.3f, 0.3f, 0.3f);
    phongShader->pointLight.diffuse = Vec3(0.6f, 0.6f, 0.6f);
    phongShader->pointLight.constant = 1.0f;
    phongShader->pointLight.linear = 0.09f;
    phongShader->pointLight.quadratic = 0.032f;

    pipeline->SetShader(phongShader);

    // Plane1
    renderer.Box(Vec3::Zero(), Vec3::Identity(), Vec3());
    phongShader->model = Matrix4::Translate(0, -1.15, 0) * Matrix4::Scale(5, 0.3, 3);
    phongShader->material.ambient = Vec3(0.43f, 0.43f, 0.43f);
    phongShader->material.diffuse = Vec3(0.43f, 0.43f, 0.43f);
    phongShader->material.specular = Vec3(0.5f, 0.5f, 0.5f);
    phongShader->material.shininess = 16;
    renderer.Render(window->getDevice(), pipeline);
    renderer.Clear();

    // Plane2
    renderer.Box(Vec3::Zero(), Vec3::Identity(), Vec3());
    phongShader->model = Matrix4::Translate(0, 0, -1.5) * Matrix4::RotateByXAxis(-90 * Math::RAD) * Matrix4::Scale(5, 0.3, 3);
    renderer.Render(window->getDevice(), pipeline);
    renderer.Clear();

    // Plane3
    renderer.Box(Vec3::Zero(), Vec3::Identity(), Vec3());
    phongShader->model = Matrix4::Translate(0, 1.15, 0) * Matrix4::Scale(5, 0.3, 3);
    renderer.Render(window->getDevice(), pipeline);
    renderer.Clear();

    // Cube
    renderer.Box(Vec3(), Vec3(1, 1, 1), Vec3());
    phongShader->model = Matrix4::Translate(-1.5, -0.5, 0);
    phongShader->material.ambient = Vec3(0.31f, 0.6f, 1.0f);
    phongShader->material.diffuse = Vec3(0.31f, 0.6f, 1.0f);
    phongShader->material.specular = Vec3(0.5f, 0.5f, 0.5f);
    phongShader->material.shininess = 32;
    renderer.Render(window->getDevice(), pipeline);
    renderer.Clear();

    // Sphere
    renderer.Sphere(Vec3(), 1, 50, Vec3());
    phongShader->model = Matrix4::Translate(0, -0.5, 0);
    phongShader->material.ambient = Vec3(1.0f, 0.5f, 0.31f);
    phongShader->material.diffuse = Vec3(1.0f, 0.5f, 0.31f);
    phongShader->material.specular = Vec3(0.5f, 0.5f, 0.5f);
    phongShader->material.shininess = 32;
    renderer.Render(window->getDevice(), pipeline);
    renderer.Clear();

    // tetrahedron
    renderer.Tetrahedron(Vec3());
    phongShader->model = Matrix4::Translate(1.5, -1, 0) * Matrix4::Scale(1.5, 1.5, 1.5);
    phongShader->material.ambient = Vec3(0.0f, 0.83f, 0.21f);
    phongShader->material.diffuse = Vec3(0.0f, 0.83f, 0.21f);
    phongShader->material.specular = Vec3(0.5f, 0.5f, 0.5f);
    phongShader->material.shininess = 32;
    renderer.Render(window->getDevice(), pipeline);
    renderer.Clear();
}

int main()
{
    window = Window::GenerateWindow(806, 829);
    pipeline= new ShaderPipeline;
    colorShader = new ColorShader;
    phongShader = new PhongShader;
    

    //pipeline->SetBackCulled(true);
    //pipeline->SetFastSampled(true);

    window->Show();
    std::thread renderThread(RenderProc);

    while (!window->ShouldClose())
    {
        window->Draw();
    }

    renderThread.join();

    delete colorShader;
    delete phongShader;
    delete pipeline;
    Window::DestroyWindow(window);
    return 0;
}
