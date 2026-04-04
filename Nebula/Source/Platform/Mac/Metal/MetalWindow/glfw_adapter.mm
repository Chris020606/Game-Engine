#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "glfw_adapter.h"

NS::Window* get_ns_window(GLFWwindow* glfwWindow, CA::MetalLayer* layer) {
    CALayer* obj_layer = (__bridge CALayer*) layer;
    NSWindow* obj_window = glfwGetCocoaWindow(glfwWindow);

    obj_window.contentView.layer = obj_layer;
    obj_window.contentView.wantsLayer = YES;

    return (__bridge NS::Window*)obj_window;
}