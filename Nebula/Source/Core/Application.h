//
// Created by Christian Cruz on 12/18/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include "Window.h"
#include "LayerStack.h"
#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Renderer/VertexArray.h"
#include "Source/Renderer/Render.h"


namespace Nebula {
    class iEvent;
    class ImGuiLayer;
    class Renderer;
    class Layer;
    class LayerStack;
}

namespace Nebula {

    struct ApplicationSpecification {
        WindowSpecification window_specification;
    };

    class Application {
        public:
        explicit Application(const ApplicationSpecification& specification = ApplicationSpecification());
        ~Application();

        void Run();
        static Application& Get();
        void RaiseEvent(iEvent& event);

        LayerStack& GetLayerStack();
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        void OnFrame();

        WindowSpecification GetSpecification() {return m_appSpec.window_specification;}
        float GetWidht() {return m_appSpec.window_specification.width;}
        float GetHeight() {return m_appSpec.window_specification.height;}
        Window* GetWindow() { return m_window.get(); }
        ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
        bool ImGuiEnabled() const { return m_appSpec.window_specification.IsImGuiEnabled; }
        void SetImGuiEnabled(bool enabled) { m_appSpec.window_specification.IsImGuiEnabled = enabled; }
        void InitImGui();
        Renderer* GetRenderer() const { return m_Renderer; }
        void OnImGuiFrame();

        private:
        std::unique_ptr<Window> m_window;
        const ApplicationSpecification m_appSpec;
        LayerStack* m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
        Renderer* m_Renderer = nullptr;
        RendererAPI* m_RendererAPI = nullptr;

        void* metalAPI = nullptr;
        void* device = nullptr;
        void* cmdQueue = nullptr;
        void* nsWindow = nullptr;

        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<VertexArray> m_VertexArray;
        Ref<Shader> m_Shader;

    };

} // Nebula

#endif //APPLICATION_H
