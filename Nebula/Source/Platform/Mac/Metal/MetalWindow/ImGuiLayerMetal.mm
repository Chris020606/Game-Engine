//
// Created by Christian Cruz on 3/24/26.
//

//#include <GLFW/glfw3.h>
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_metal.h"
//#include "ImGuiLayerMetal.h"
//
//#import <Metal/Metal.h>
//#import <QuartzCore/QuartzCore.h>
//
//#include "Source/Renderer/Render.h"
//
//#include "Source/Platform/Mac/Metal/MetalAPI/MetalRendererAPI.h"


// THEN: normal Apple frameworks
#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

// THEN: your stuff
#include <GLFW/glfw3.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include "ImGuiLayerMetal.h"

#include "Source/Renderer/Render.h"
#include "Source/Platform/Mac/Metal/MetalAPI/MetalRendererAPI.h"



namespace Nebula {
    ImGuiLayerMetal::ImGuiLayerMetal() {

    }

    ImGuiLayerMetal::~ImGuiLayerMetal() {
    }

    void ImGuiLayerMetal::Init(void *window) {
        m_window = static_cast<GLFWwindow*>(window);
        m_Device = MetalRendererAPI::GetDevice();



        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Scale for retina
        float scale = 1.0f;
        glfwGetWindowContentScale(m_window, &scale, nullptr);

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplMetal_Init((__bridge id<MTLDevice>) m_Device);

\
    }

    bool ImGuiLayerMetal::NewFrame() {
    auto* renderer = static_cast<MetalRendererAPI*>(Renderer::GetRendererAPI());
        if (!renderer) return false;

        m_CommandBuffer = renderer->GetCommandBuffer();
        m_Encoder       = MetalRendererAPI::GetEncoder();


        if (!m_CommandBuffer || !m_Encoder) return false;

        MTLRenderPassDescriptor* rpd = [MTLRenderPassDescriptor new];
        rpd.colorAttachments[0].loadAction  = MTLLoadActionLoad;
        rpd.colorAttachments[0].storeAction = MTLStoreActionStore;
        rpd.colorAttachments[0].texture     = (__bridge id<MTLTexture>)
                                          renderer->GetCurrentDrawable()->texture();

        rpd.colorAttachments[0].resolveTexture = (__bridge id<MTLTexture>)
                                              renderer->GetCurrentDrawable()->texture();


        ImGui_ImplMetal_NewFrame(rpd);
        ImGui_ImplGlfw_NewFrame();

        return true;
    }

    void ImGuiLayerMetal::RenderDrawData() {
        if (!m_CommandBuffer || !m_Encoder) return;

        ImGui_ImplMetal_RenderDrawData(
            ImGui::GetDrawData(),
            (__bridge id<MTLCommandBuffer>)        m_CommandBuffer,
            (__bridge id<MTLRenderCommandEncoder>) m_Encoder
        );
    }

    void ImGuiLayerMetal::Shutdown() {
        ImGui_ImplMetal_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    ImVec2 ImGuiLayerMetal::GetDisplaySize() const {
        int w, h;
        glfwGetWindowSize(m_window, &w, &h);
        return ImVec2((float)w, (float)h);
    }
} // Nebula