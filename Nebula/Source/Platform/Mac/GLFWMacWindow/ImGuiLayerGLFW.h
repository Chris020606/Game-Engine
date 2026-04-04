//
// Created by Christian Cruz on 1/8/26.
//

#ifndef IMGUILAYERGLFW_H
#define IMGUILAYERGLFW_H
#include "ImGuiLayer.h"
#include "WindowMac.h"

struct ImVec2;

namespace Nebula {

     class ImGuiOpenGLRenderer : public ImGuiRenderer {
     public:
          ImGuiOpenGLRenderer();
          ~ImGuiOpenGLRenderer();

          void Init(void *window) override;
          bool NewFrame() override;
          void RenderDrawData() override;
          void Shutdown() override;

          ImVec2 GetDisplaySize() const override;

     private:
          GLFWwindow* m_Window = nullptr;
     };

} // Nebula
#endif //IMGUILAYERGLFW_H
