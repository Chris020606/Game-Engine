#include <Nebula.h>
#include "App.h"
#include "Editor/EditorLayer.h"

int main() {
    Nebula::ApplicationSpecification spec;
    spec.window_specification.width = 1200;
    spec.window_specification.height = 800;
    spec.window_specification.windowType = Nebula::WindowType::Metal;
    spec.window_specification.api = Nebula::API::Metal;
    spec.window_specification.IsImGuiEnabled = false;
    App app(spec);
    app.Run();
    return 0;
}