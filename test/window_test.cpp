#include "immgraphics.h"

using namespace ImmGraphics;

int main()
{
    Window* window = Window::GenerateWindow(800, 600);

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(new ShaderPipeline);
    window->Show();

    while (!window->ShouldClose())
    {
        window->ClearBuffer(0xFF0000FF);
        renderer.Box(Transform::Default());
        renderer.Render();
        window->Draw();
    }

    Window::DestroyWindow(window);
    return 0;
}
