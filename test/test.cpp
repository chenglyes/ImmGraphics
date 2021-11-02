#include "renderer.h"

using namespace ImmGraphics;

int main()
{
    auto device = RenderDevice::CreateDevice("", 0, 0);
    Renderer renderer(device);
    renderer.AddPipeline(new ShaderPipeline);

    renderer.Box(Transform::Default());

    RenderDevice::DestroyDevice(device);
    return 0;
}