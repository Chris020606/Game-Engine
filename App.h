//
// Created by Christian Cruz on 12/18/25.
//

#ifndef APP_H
#define APP_H
#include <Nebula.h>
#include <glm/glm.hpp>


#include "imgui.h"
#include "TextEditor.h"
#include "Source/Core/Events/InputEvents.h"
#include "Source/Core/Events/WindowEvent.h"


class TestLayer : public Nebula::Layer {
    public:
    void OnAttach() override;
    void OnDetach() override;

    void OnEvent(Nebula::iEvent &event) override;
    void OnRender() override;
    void OnUpdate(float delta) override;

    void OnImGuiRender() override;

    static bool ImGuiEnable() { return s_EnableImGuI; }

private:
    bool OnMouseMoved(Nebula::MouseMovedEvent& event);
    bool OnMouseScrolled(Nebula::MouseScrolledEvent& event);
    bool OnMouseButtonPressed(Nebula::MouseButtonPressedEvent& event);
    bool OnMouseButtonReleased(Nebula::MouseButtonReleasedEvent& event);
    bool OnKeyPressed(Nebula::KeyPressedEvent& event);
    bool OnKeyReleased(Nebula::KeyReleasedEvent& event);
    bool OnWindowClosed(Nebula::WindowClosedEvent& event);

    glm::vec2 m_MousePosition{ 0.0f };
    float m_CameraYaw   = 0.f;
    float m_CameraPitch = 0.f;
    float m_CameraSpeed = 0.1f;
    float m_MouseSensitivity = 0.1f;
    bool  m_FirstMouse  = true;
    float m_LastMouseX  = 0.f;
    float m_LastMouseY  = 0.f;
    bool  m_RightMouseDown = false;
    std::unique_ptr<Nebula::Renderer> m_Renderer;
    ImVec2 m_ViewportSize{0.0f, 0.0f};

    std::unordered_map<int, bool> m_Keys;

    TextEditor editor;

    bool m_Initialized = false;
    static bool s_EnableImGuI;
    float m_CubeAngle = 0.f;
    Ref<Nebula::Mesh>     m_CubeMesh;
    Ref<Nebula::Shader>   m_CubeShader;
    Ref<Nebula::Material> m_CubeMaterial;
    Ref<Nebula::Camera>   m_Camera;
    Ref<Nebula::Framebuffer> m_ViewportFramebuffer;
    Ref<Nebula::Model>    m_Model;
    Ref<Nebula::Shader>        m_ModelShader;
    Ref<Nebula::Material> m_ModelMaterial;
    Ref<Nebula::ComputeShader> m_RippleShader;
    Ref<Nebula::Texture2D>     m_RippleTexture;
    Ref<Nebula::Mesh>          m_QuadMesh;
    Ref<Nebula::Shader>        m_QuadShader;
    Ref<Nebula::Material>      m_QuadMaterial;
    Ref<Nebula::Model>         m_DragonModel;
    Ref<Nebula::Shader>        m_DragonShader;
    Ref<Nebula::Material>      m_DragonMaterial;
    float                      m_Time = 0.f;

};

class App : public Nebula::Application {
public:
    explicit App(const Nebula::ApplicationSpecification& spec);
    ~App() = default;

private:
    TestLayer* m_TestLayer;
};



#endif //APP_H
