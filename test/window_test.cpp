#include "immgraphics.h"

using namespace ImmGraphics;

int main()
{
    Window* window = Window::GenerateWindow(800, 600);

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(new ShaderPipeline);

    renderer.Box(Transform::Default());
    renderer.Render();

    window->Show();

    while (!window->ShouldClose())
    {
        
    }

    Window::DestroyWindow(window);
    return 0;
}
