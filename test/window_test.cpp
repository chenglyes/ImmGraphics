#include "immgraphics.h"

using namespace ImmGraphics;

int main()
{
    Window* window = Window::GenerateWindow(800, 600);

    Renderer renderer(window->getDevice());
    renderer.AddPipeline(new ShaderPipeline);

    window->ClearBuffer(0x1E2227);
    
    renderer.Box(Transform::Default());
    renderer.Render();

    window->Show();

    while (!window->ShouldClose())
    {
        
        // window->Draw();
    }

    Window::DestroyWindow(window);
    return 0;
}
