//
// Created by Christian Cruz on 3/21/26.
//

#ifndef METALWINDOW_H
#define METALWINDOW_H

#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Core/Window.h"
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include "GLFW/glfw3.h"

namespace Nebula {

class MetalWindow : public Window {
    public:
    MetalWindow( WindowSpecification spec);
    ~MetalWindow() override;

    void Update() override;
    bool ShouldClose() const override;
    float GetTime() override;

    void SetEventCallback(const EventCallbackFn& callback) override;
    void RaiseEvent(iEvent& event) const;
    void SetApplication(Application *app) override;

    float GetHeight() const override;
    float GetWidth() const override;

    void* GetNativeWindow() override { return m_GLFWwindow; }

    ImVec2 GetFramebufferSize() const override { return {};}

    Ref<ImGuiRenderer> CreateImGuiRenderer() override;

    private:
        GLFWwindow* m_GLFWwindow;
        NS::Window* m_NSWindow;
        CA::MetalLayer* m_MetalLayer;
        MTL::Device* m_Device;
        Application* m_App = nullptr;
        WindowSpecification m_WindowSpec;

        bool m_ImGuiInitialized = false;
        void SetCallBacks();

};

} // Nebula

#endif //METALWINDOW_H
