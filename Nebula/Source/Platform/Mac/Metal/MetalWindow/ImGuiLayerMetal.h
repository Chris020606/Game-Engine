//
// Created by Christian Cruz on 3/24/26.
//

#ifndef IMGUILAYERMETAL_H
#define IMGUILAYERMETAL_H

#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <QuartzCore/QuartzCore.hpp>


namespace Nebula {

class ImGuiLayerMetal  : public ImGuiRenderer {
    public:

    ImGuiLayerMetal();
    ~ImGuiLayerMetal() override;

    void Init(void *window) override;
    bool NewFrame() override;
    void RenderDrawData() override;
    void Shutdown() override;

    ImVec2 GetDisplaySize() const override;

    private:
        GLFWwindow* m_window;
        MTL::Device* m_Device;
        MTL::CommandQueue* m_Queue;
        MTL::CommandBuffer*        m_CommandBuffer = nullptr;
        MTL::RenderCommandEncoder* m_Encoder       = nullptr;

};

} // Nebula

#endif //IMGUILAYERMETAL_H
