//
// Created by Christian Cruz on 12/30/25.
//
#include "Source/Core/Window.h"
#include "Mac/GLFWMacWindow/WindowMac.h"
#include "Mac/Metal/MetalWindow/MetalWindow.h"
// #include "Mac/Metal/MetalWindow/MetalWindow.h"

namespace Nebula {

    std::unique_ptr<Window> Window::Create(const WindowSpecification& spec)
    {
        switch (spec.windowType)
        {
            case WindowType::GLFW:
                std::cout << "WindowType::GLFW" << std::endl;
                return std::make_unique<WindowMac>(spec);
            case WindowType::Metal:
                std::cout << "WindowType::Metal" << std::endl;
                 return std::make_unique<MetalWindow>(spec);
            case WindowType::None:
                std::cout << "WindowType::None" << std::endl;
                return nullptr;
        }
    }

}