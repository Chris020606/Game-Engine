//
// Created by Christian Cruz on 3/21/26.
//

#ifndef GLFW_ADAPTER_H
#define GLFW_ADAPTER_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace CA { class MetalLayer; }
namespace NS { class Window; }

NS::Window* get_ns_window(GLFWwindow* glfwWindow, CA::MetalLayer* layer);

#endif //GLFW_ADAPTER_H
