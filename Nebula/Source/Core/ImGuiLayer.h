//
// Created by Christian Cruz on 1/8/26.
//

#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include <memory>

#include "Layer.h"
#include <imgui.h>

#include "Core.h"

namespace Nebula {
    class ImGuiRenderer {
    public:
        virtual ~ImGuiRenderer() = default;
        virtual void Init(void *window) = 0;
        virtual bool NewFrame() = 0;
        virtual void RenderDrawData() = 0;
        virtual void Shutdown() = 0;

        virtual ImVec2 GetDisplaySize() const = 0;
    };

    class ImGuiLayer : public Layer {
    public:
        explicit ImGuiLayer(Ref<ImGuiRenderer> backend);
         ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

        virtual void Begin();
        virtual void End();
        void RenderToGPU();

        void BeginDockspace();
        void EndDockspace();

        static ImGuiLayer* Get();
        void DrawViewport(void* texture, ImVec2 size);

    protected:
        Ref<ImGuiRenderer> m_Backend;
        bool m_DockspaceOpen = true;
    };
}


#endif //IMGUILAYER_H
