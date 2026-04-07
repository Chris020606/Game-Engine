//
// Created by Christian Cruz on 12/18/25.
//

#include "App.h"

#include <iostream>

#include "Framebuffers.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "Editor/ThemeManager.h"

#define KEY_W  87
#define KEY_A  65
#define KEY_S  83
#define KEY_D  68
#define KEY_Q  81
#define KEY_E  69
#define KEY_I  73

bool TestLayer::s_EnableImGuI = true;

void TestLayer::OnAttach() {
    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
    editor.SetShowWhitespaces(false);

    m_Camera = std::make_shared<Nebula::Camera>(glm::vec3(0.f, 5.f, -5.f));
    m_Camera->SetAspectRatio(16.f / 9.f);

    Nebula::FramebufferSpecification spec;
    spec.Width    = 1280;
    spec.Height   = 720;
    spec.HasDepth = true;
    m_ViewportFramebuffer = Nebula::Framebuffer::Create(spec);
}

void TestLayer::OnDetach() {

}

void TestLayer::OnUpdate(float delta) {
    if (!m_Camera) return;

    float yawRad = glm::radians(m_CameraYaw);
    glm::vec3 forward = glm::normalize(glm::vec3( sinf(yawRad), 0.f, cosf(yawRad)));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));

    glm::vec3 pos = m_Camera->GetPosition();

    if (m_Keys[KEY_W]) pos += forward * m_CameraSpeed;
    if (m_Keys[KEY_S]) pos -= forward * m_CameraSpeed;
    if (m_Keys[KEY_A]) pos -= right   * m_CameraSpeed;
    if (m_Keys[KEY_D]) pos += right   * m_CameraSpeed ;
    if (m_Keys[KEY_E]) pos.y += m_CameraSpeed;
    if (m_Keys[KEY_Q]) pos.y -= m_CameraSpeed;

    m_Camera->SetPosition(pos);
    m_Time += delta / 60.0f;

}

void TestLayer::OnRender() {
    if (!m_Initialized) {
        m_Initialized = true;

        const float s = 0.5f;
            std::vector<Nebula::Vertex> verts = {
                // Front face
                { {-s, -s, +s}, {0.f, 0.f, 1.f}, {0.f, 1.f}, {1.f,1.f,1.f} },
                { {+s, -s, +s}, {0.f, 0.f, 1.f}, {1.f, 1.f}, {1.f,1.f,1.f} },
                { {+s, +s, +s}, {0.f, 0.f, 1.f}, {1.f, 0.f}, {1.f,1.f,1.f} },
                { {-s, +s, +s}, {0.f, 0.f, 1.f}, {0.f, 0.f}, {1.f,1.f,1.f} },

                // Right face
                { {+s, -s, +s}, {1.f, 0.f, 0.f}, {0.f, 1.f}, {1.f,1.f,1.f} },
                { {+s, -s, -s}, {1.f, 0.f, 0.f}, {1.f, 1.f}, {1.f,1.f,1.f} },
                { {+s, +s, -s}, {1.f, 0.f, 0.f}, {1.f, 0.f}, {1.f,1.f,1.f} },
                { {+s, +s, +s}, {1.f, 0.f, 0.f}, {0.f, 0.f}, {1.f,1.f,1.f} },

                // Back face
                { {+s, -s, -s}, {0.f, 0.f,-1.f}, {0.f, 1.f}, {1.f,1.f,1.f} },
                { {-s, -s, -s}, {0.f, 0.f,-1.f}, {1.f, 1.f}, {1.f,1.f,1.f} },
                { {-s, +s, -s}, {0.f, 0.f,-1.f}, {1.f, 0.f}, {1.f,1.f,1.f} },
                { {+s, +s, -s}, {0.f, 0.f,-1.f}, {0.f, 0.f}, {1.f,1.f,1.f} },

                // Left face
                { {-s, -s, -s}, {-1.f,0.f,0.f}, {0.f, 1.f}, {1.f,1.f,1.f} },
                { {-s, -s, +s}, {-1.f,0.f,0.f}, {1.f, 1.f}, {1.f,1.f,1.f} },
                { {-s, +s, +s}, {-1.f,0.f,0.f}, {1.f, 0.f}, {1.f,1.f,1.f} },
                { {-s, +s, -s}, {-1.f,0.f,0.f}, {0.f, 0.f}, {1.f,1.f,1.f} },

                // Top face
                { {-s, +s, +s}, {0.f, 1.f,0.f}, {0.f, 1.f}, {1.f,1.f,1.f} },
                { {+s, +s, +s}, {0.f, 1.f,0.f}, {1.f, 1.f}, {1.f,1.f,1.f} },
                { {+s, +s, -s}, {0.f, 1.f,0.f}, {1.f, 0.f}, {1.f,1.f,1.f} },
                { {-s, +s, -s}, {0.f, 1.f,0.f}, {0.f, 0.f}, {1.f,1.f,1.f} },

                // Bottom face
                { {-s, -s, -s}, {0.f,-1.f,0.f}, {0.f, 1.f}, {1.f,1.f,1.f} },
                { {+s, -s, -s}, {0.f,-1.f,0.f}, {1.f, 1.f}, {1.f,1.f,1.f} },
                { {+s, -s, +s}, {0.f,-1.f,0.f}, {1.f, 0.f}, {1.f,1.f,1.f} },
                { {-s, -s, +s}, {0.f,-1.f,0.f}, {0.f, 0.f}, {1.f,1.f,1.f} },
            };

            std::vector<uint32_t> indices = {
                0, 1, 2,  2, 3, 0,
                4, 5, 6,  6, 7, 4,
                8, 9,10, 10,11, 8,
               12,13,14, 14,15,12,
               16,17,18, 18,19,16,
               20,21,22, 22,23,20,
           };

           std::vector<Nebula::Vertex> qv = {
            { {-1,-1,0}, {0,0,1}, {0,0}, {1,1,1} },
            { {+1,-1,0}, {0,0,1}, {1,0}, {1,1,1} },
            { {+1,+1,0}, {0,0,1}, {1,1}, {1,1,1} },
            { {-1,+1,0}, {0,0,1}, {0,1}, {1,1,1} },
            };

            std::vector<uint32_t> qi = { 0,2,1, 2,0,3 };

            Nebula::TextureParams params;
            params.Filter = Nebula::TextureFilter::Linear;
            params.Wrap = Nebula::TextureWrap::Repeat;

            m_CubeMesh = std::make_shared<Nebula::Mesh>(verts, indices);
            m_QuadMesh = std::make_shared<Nebula::Mesh>(qv, qi);
            m_Model = Nebula::Model::Create("/Users/christiancruz/CLionProjects/NebulaEngine/App/WoodenExport/Wooden.obj");
            m_DragonModel = Nebula::Model::Create("/Users/christiancruz/CLionProjects/NebulaEngine/App/Stanford Dragon PBR.glb");

            Nebula::TextureParams param;
            param.Filter = Nebula::TextureFilter::Linear;
            param.Wrap   = Nebula::TextureWrap::Clamp;

            m_CubeShader = Nebula::Shader::Create("/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Platform/Mac/Metal/MetalAPI/shader.metal");
            m_QuadShader = Nebula::Shader::Create("/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Platform/Mac/Metal/MetalAPI/shader.metal");
            m_ModelShader = Nebula::Shader::Create("/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Platform/Mac/Metal/MetalAPI/shader.metal");
            m_DragonShader = Nebula::Shader::Create("/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Platform/Mac/Metal/MetalAPI/dragon.metal");
            m_RippleShader = Nebula::ComputeShader::Create("/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Platform/Mac/Metal/MetalAPI/compute.metal");
            m_RippleShader->SetFloat("time", 0.f);
            m_RippleShader->Dispatch(512, 512);

            m_CubeMaterial = std::make_shared<Nebula::Material>( m_CubeShader, std::unordered_map<std::string, Ref<Nebula::Texture>>{});
            m_QuadMaterial = std::make_shared<Nebula::Material>(m_QuadShader, std::unordered_map<std::string, Ref<Nebula::Texture>>{});
            m_ModelMaterial = std::make_shared<Nebula::Material>(m_ModelShader, std::unordered_map<std::string, Ref<Nebula::Texture>>{});
            m_DragonMaterial = std::make_shared<Nebula::Material>(m_DragonShader, std::unordered_map<std::string, Ref<Nebula::Texture>>{});

            auto cubeTexture = Nebula::Texture2D::Create("/Users/christiancruz/CLionProjects/NebulaEngine/Nebula.png");
            auto tex = Nebula::Texture2D::Create("/Users/christiancruz/CLionProjects/NebulaEngine/App/WoodenExport/WoodenTexture.png", params);
            m_RippleTexture = Nebula::Texture2D::WrapNative(m_RippleShader->GetOutputTexture(), param);

            m_QuadMaterial->SetTexture("tex", m_RippleTexture);
            m_CubeMaterial->SetTexture("tex", cubeTexture); // TODO If the user passes none texture it will crash fix it
            m_ModelMaterial->SetTexture("tex", tex);
            m_DragonMaterial->SetTexture("tex", m_RippleTexture);

            m_ModelMaterial->SetDoubleSided(true);
            m_QuadMaterial->SetDoubleSided(true);

    }
        if (m_RippleShader) {
            m_RippleShader->SetFloat("time", m_Time);
            m_RippleShader->Dispatch(512, 512);
        }

        Nebula::RenderCommand::BeginRenderPass();

        glm::mat4 modelCube = glm::translate(glm::mat4(1.f), glm::vec3(0,-3,5));
        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(0,-3,20));
        glm::mat4 quadModel = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -3.f, 0.f));
        glm::mat4 dragonModel = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -3.f, 40.f));

        if (m_ViewportFramebuffer && s_EnableImGuI)
            Nebula::RenderCommand::RenderToFramebuffer(m_ViewportFramebuffer);

        Nebula::RenderCommand::SetClearColor({ 1.f, 1.f, 1.f, 1.f });
        Nebula::RenderCommand::Clear();

        Nebula::Renderer::BeginScene(m_Camera);

        Nebula::Renderer::Submit(m_QuadMesh, m_QuadMaterial, quadModel);
        Nebula::Renderer::Submit(m_CubeMesh, m_CubeMaterial, modelCube);
        Nebula::Renderer::SubmitModel(m_Model, m_ModelMaterial, model);
        Nebula::Renderer::SubmitModel(m_DragonModel, m_DragonMaterial, dragonModel);

        Nebula::Renderer::EndScene();
}

void TestLayer::OnImGuiRender() {

     ImGui::Begin("Settings");
     ImGui::End();

    ImGui::Begin("Script Editor");
    editor.Render("TextEditor");
    ImGui::End();


     if (ImGui::Begin("Theme Settings"))
     {
         if (ImGui::Button("Dark Theme"))                ThemeManager::ApplyDarkTheme();
         if (ImGui::Button("Light Theme"))               ThemeManager::ApplyLightTheme();
         if (ImGui::Button("Modern Dark"))               ThemeManager::ApplyModernDark();
         if (ImGui::Button("Blender Theme"))             ThemeManager::ApplyBlenderTheme();
         if (ImGui::Button("Photoshop Theme"))           ThemeManager::ApplyPhotoshopDark();
         if (ImGui::Button("Unity Theme"))               ThemeManager::ApplyUnityTheme();
         if (ImGui::Button("Windows Theme"))             ThemeManager::ApplyWindows11Fluent();
         if (ImGui::Button("Flat Pastel Theme"))         ThemeManager::ApplyFlatPastel();
         if (ImGui::Button("Godot Theme"))               ThemeManager::ApplyGodotTheme();

     }
     ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    {
         ImVec2 viewportSize = ImGui::GetContentRegionAvail();

         if (m_ViewportFramebuffer && viewportSize.x > 0 && viewportSize.y > 0) {
             auto& spec = m_ViewportFramebuffer->GetSpecification();

             if (spec.Width  != (uint32_t)viewportSize.x ||
                 spec.Height != (uint32_t)viewportSize.y) {
                 m_ViewportFramebuffer->Resize(
                     (uint32_t)viewportSize.x,
                     (uint32_t)viewportSize.y
                 );
                 m_Camera->SetAspectRatio(viewportSize.x / viewportSize.y);
                 }

             void* texID = m_ViewportFramebuffer->GetColorAttachmentRendererID();
             m_ViewportFramebuffer->SetClearColor(1.f, 1.f, 1.f, 1.f);
             if (texID)
                 ImGui::Image((ImTextureID)texID, viewportSize,
                              ImVec2(0, 0), ImVec2(1, 1));
             else
                 ImGui::Text("No texture");
         } else {
             ImGui::Text("Framebuffer not ready");
         }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void TestLayer::OnEvent(Nebula::iEvent& event)
{
    Nebula::EventDispatcher d(event);
    d.Dispatch<Nebula::MouseMovedEvent>(           [this](auto& e){ return OnMouseMoved(e);           });
    d.Dispatch<Nebula::MouseScrolledEvent>(        [this](auto& e){ return OnMouseScrolled(e);        });
    d.Dispatch<Nebula::MouseButtonPressedEvent>(   [this](auto& e){ return OnMouseButtonPressed(e);   });
    d.Dispatch<Nebula::MouseButtonReleasedEvent>(  [this](auto& e){ return OnMouseButtonReleased(e);  });
    d.Dispatch<Nebula::KeyPressedEvent>(           [this](auto& e){ return OnKeyPressed(e);           });
    d.Dispatch<Nebula::KeyReleasedEvent>(          [this](auto& e){ return OnKeyReleased(e);          });
    d.Dispatch<Nebula::WindowClosedEvent>(         [this](auto& e){ return OnWindowClosed(e);         });
    // std::cout << event.ToString() << std::endl;
}

bool TestLayer::OnKeyPressed(Nebula::KeyPressedEvent& event)
{
    m_Keys[event.GetKeyCode()] = true;
    if (event.GetKeyCode() == KEY_I) {
        s_EnableImGuI = !s_EnableImGuI;
    }
    return false;
}

bool TestLayer::OnKeyReleased(Nebula::KeyReleasedEvent& event)
{
    m_Keys[event.GetKeyCode()] = false;
    return false;
}

bool TestLayer::OnMouseButtonPressed(Nebula::MouseButtonPressedEvent& event)
{
    if (event.GetMouseButton() == 1) { // right mouse button
        m_RightMouseDown = true;
        m_FirstMouse     = true;
    }
    return false;
}

bool TestLayer::OnMouseButtonReleased(Nebula::MouseButtonReleasedEvent& event)
{
    if (event.GetMouseButton() == 1)
        m_RightMouseDown = false;
    return false;
}

bool TestLayer::OnMouseMoved(Nebula::MouseMovedEvent& event)
{
    float x = static_cast<float>(event.GetX());
    float y = static_cast<float>(event.GetY());
    m_MousePosition = { x, y };

    if (!m_RightMouseDown) {
        m_LastMouseX = x;
        m_LastMouseY = y;
        return false;
    }

    if (m_FirstMouse) {
        m_LastMouseX = x;
        m_LastMouseY = y;
        m_FirstMouse = false;
        return false;
    }

    float dx =  (x - m_LastMouseX) * m_MouseSensitivity;
    float dy =  -(y - m_LastMouseY) * m_MouseSensitivity;
    m_LastMouseX = x;
    m_LastMouseY = y;

    m_CameraYaw   += dx;
    m_CameraPitch -= dy;
    m_CameraPitch  = glm::clamp(m_CameraPitch, -89.f, 89.f);

    m_Camera->SetRotation(glm::vec3(m_CameraPitch, m_CameraYaw, 0.f));
    return false;
}

bool TestLayer::OnMouseScrolled(Nebula::MouseScrolledEvent& event)
{
    if (!m_Camera) return false;

    float yawRad = glm::radians(m_CameraYaw);
    glm::vec3 forward = glm::normalize(glm::vec3(
        sinf(yawRad), 0.f, cosf(yawRad)));

    glm::vec3 pos = m_Camera->GetPosition();
    pos += forward * (float)event.GetYOffset() * 0.5f;
    m_Camera->SetPosition(pos);
    return false;
}

bool TestLayer::OnWindowClosed(Nebula::WindowClosedEvent& event)
{
    std::println("Window Closed!");
    return false;
}

App::App(const Nebula::ApplicationSpecification& spec)
    : Application(spec)
{
    TestLayer* test = new TestLayer();
    PushLayer(test);
}
