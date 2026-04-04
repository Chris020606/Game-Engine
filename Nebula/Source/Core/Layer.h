//
// Created by Christian Cruz on 12/22/25.
//

#ifndef LAYER_H
#define LAYER_H

#include "Events/iEvent.h"
#include <memory>

namespace Nebula {

class Layer {
    public:

        virtual ~Layer() = default;

        virtual void OnUpdate(float ts) {}
        virtual void OnRender() {}
        virtual void OnEvent(iEvent& event) {}
        virtual void OnAttach() {}
        virtual void OnDetach() {}


        template<std::derived_from<Layer> T, typename... Args>
        void Transition(Args&& ... args) {
            QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args) ... )));
        }
        virtual void Suspend() {}
        virtual void OnImGuiRender() {}

    private:
        void QueueTransition(std::unique_ptr<Layer> layer);

};

} // Nebula

#endif //LAYER_H


// void EditorLayer::OnImGuiRender()
// {
//     ImGui::SetNextWindowPos({0, 0});
//     ImGui::SetNextWindowSize({m_WindowWidth, 32});
//
//     ImGui::Begin("##TopBar",
//         nullptr,
//         ImGuiWindowFlags_NoDecoration |
//         ImGuiWindowFlags_NoMove |
//         ImGuiWindowFlags_NoDocking
//     );
//
//     ImGui::Button("Play");
//     ImGui::SameLine();
//     ImGui::Button("Stop");
//
//     ImGui::End();
// }