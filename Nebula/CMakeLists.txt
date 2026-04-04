cmake_minimum_required(VERSION 3.31)

project(NebulaEngine LANGUAGES C CXX)

# ---------------- metal-cpp paths ----------------
# Adjust this path if metal-cpp is located elsewhere
set(METAL_CPP_DIR ${CMAKE_SOURCE_DIR}/extern/metal-cpp)
set(METAL_CPP_EXT_DIR ${CMAKE_SOURCE_DIR}/extern/metal-cpp-extensions)

# ---------------- C++ Standard ----------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------- Nebula Library ----------------
add_library(Nebula STATIC
        Source/Core/Application.cpp
        Source/Platform/Mac/GLFWMacWindow/WindowMac.cpp
        Source/Renderer/Render.cpp
        Source/Renderer/Render.h
        Source/Renderer/RendererAPI.cpp
        Source/Renderer/RendererAPI.h
        Source/Platform/OpenGlAPI/OpenGLRendererAPI.cpp
        Source/Platform/OpenGlAPI/OpenGLRendererAPI.h
        Source/Core/Events/iEvent.h
        Source/Core/Layer.cpp
        Source/Core/Layer.h
        Source/Core/LayerStack.cpp
        Source/Core/LayerStack.h
        Source/Platform/Mac/Metal/MetalAPI/MetalRendererAPI.cpp
        Source/Platform/Mac/Metal/MetalAPI/MetalRendererAPI.h
        Source/Platform/CreateWindow.cpp
        Source/Renderer/ComputeShader.cpp
        Source/Renderer/ComputeShader.h
        Source/Renderer/Shader.h
        Source/Core/ImGuiLayer.h
        Source/Platform/Mac/GLFWMacWindow/ImGuiLayerGLFW.cpp
        Source/Platform/Mac/GLFWMacWindow/ImGuiLayerGLFW.h
        Source/Core/ImGuiLayer.cpp
        Source/Renderer/Framebuffers.cpp
        Source/Renderer/Framebuffers.h
        Source/Platform/Mac/Metal/MetalAPI/MetalFramebuffer.mm
        Source/Platform/Mac/Metal/MetalAPI/MetalFramebuffer.h
        Source/Renderer/UniformBuffers.cpp
        Source/Renderer/UniformBuffers.h
        Source/Renderer/VertexBuffers.cpp
        Source/Renderer/VertexBuffers.h
        Source/Platform/OpenGlAPI/OpenGLShader.h
        Source/Platform/OpenGlAPI/OpenGLBuffer.cpp
        Source/Platform/OpenGlAPI/OpenGLBuffer.h
        Source/Platform/Mac/Metal/MetalAPI/MetalBuffers.cpp
        Source/Platform/Mac/Metal/MetalAPI/MetalBuffers.h
        Source/Renderer/VertexArray.cpp
        Source/Renderer/VertexArray.h
        Source/Platform/Mac/Metal/MetalAPI/MetalVertexArray.cpp
        Source/Platform/Mac/Metal/MetalAPI/MetalVertexArray.h
        Source/Renderer/Shader.cpp
        Source/Renderer/RenderCommand.cpp
        Source/Renderer/RenderCommand.h
        Source/Platform/Mac/Metal/MetalAPI/MetalShader.cpp
        Source/Platform/Mac/Metal/MetalAPI/MetalShader.h
        Source/Renderer/Texture.cpp
        Source/Renderer/Texture.h
        Source/Platform/Mac/Metal/MetalAPI/MetalTexture.cpp
        Source/Platform/Mac/Metal/MetalAPI/MetalTexture.h
        Source/Renderer/stb_image.cpp
        Source/Platform/Mac/Metal/MetalAPI/MetalComputeShader.cpp
        Source/Platform/Mac/Metal/MetalAPI/MetalComputeShader.h
        Source/Platform/Mac/Metal/MetalAPI/MetalUniformBuffer.cpp
        Source/Platform/Mac/Metal/MetalAPI/MetalUniformBuffer.h
        Source/Renderer/Material.cpp
        Source/Renderer/Material.h
        Source/Renderer/Transform.cpp
        Source/Renderer/Transform.h
        Source/Renderer/Cameras.cpp
        Source/Renderer/Cameras.h
        Source/Renderer/Mesh.cpp
        Source/Renderer/Mesh.h
        Source/Renderer/Model.cpp
        Source/Renderer/Model.h
        Source/Core/ID.cpp
        Source/Core/ID.h
        Source/Core/Core.h
)

target_include_directories(Nebula
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${METAL_CPP_DIR}
        ${METAL_CPP_EXT_DIR}
        ${CMAKE_SOURCE_DIR}/Nebula/Source/Core
        ${CMAKE_SOURCE_DIR}/Nebula/Source/Renderer
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/assimp/include
        SYSTEM PUBLIC
)

# ---------------- Assimp ----------------
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS         OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL             OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/assimp ${CMAKE_BINARY_DIR}/extern/assimp)

target_compile_options(assimp PRIVATE -w -Wno-error)

set_target_properties(assimp PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
        $<TARGET_PROPERTY:assimp,INTERFACE_INCLUDE_DIRECTORIES>
)


target_sources(Nebula PRIVATE
)
target_include_directories(Nebula PUBLIC
        ${METAL_CPP_DIR}
        ${METAL_CPP_EXT_DIR}
)
target_sources(Nebula PRIVATE
        Source/Platform/Mac/Metal/MetalCPPImplementation.mm
)
# ---------------- GLAD ----------------
add_library(glad STATIC
        ${CMAKE_SOURCE_DIR}/extern/glad/src/glad.c
)

target_include_directories(glad
        PUBLIC
        ${CMAKE_SOURCE_DIR}/extern/glad/include
)

# ---------------- GLFW ----------------
find_package(glfw3 3.3 REQUIRED)

# ---------------- macOS system libs ----------------
find_library(METAL Metal)
find_library(FOUNDATION Foundation)
find_library(QUARTZCORE QuartzCore)
find_library(APPKIT AppKit)

# ---------------- ImGui ----------------
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_metal.mm
        ${IMGUI_DIR}/backends/imgui_impl_osx.mm
        Source/Core/Project/ProjectManager.cpp
        Source/Core/Project/ProjectManager.h
        Source/Core/Events/iEvent.cpp
        Source/Platform/Mac/Metal/MetalWindow/MetalWindow.cpp
        Source/Platform/Mac/Metal/MetalWindow/MetalWindow.h
        Source/Platform/Mac/Metal/MetalWindow/glfw_adapter.h
        Source/Platform/Mac/Metal/MetalWindow/glfw_adapter.mm
        Source/Platform/Mac/Metal/MetalWindow/ImGuiLayerMetal.mm
        Source/Platform/Mac/Metal/MetalWindow/ImGuiLayerMetal.h
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        /opt/homebrew/include
        ${METAL_CPP_DIR}
        ${METAL_CPP_EXT_DIR}
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/assimp/include
        ${CMAKE_BINARY_DIR}/extern/assimp/include
        ${CMAKE_SOURCE_DIR}/Nebula                        # <-- add this
        ${CMAKE_SOURCE_DIR}/Nebula/Source/Core            # <-- add this
        ${CMAKE_SOURCE_DIR}/Nebula/Source/Renderer        # <-- add this
)
target_link_libraries(imgui PUBLIC assimp glfw)

# ---------------- ImGuizmo ----------------
set(IMGUIZU_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../extern/ImGuizmo)
add_library(imguizmo STATIC ${IMGUIZU_DIR}/ImGuizmo.cpp)
target_include_directories(imguizmo PUBLIC
        ${IMGUIZU_DIR}
        ${IMGUI_DIR}  # ImGuizmo needs ImGui headers
)
target_link_libraries(imguizmo PUBLIC imgui)

# Add ImGuiFileDialog
# ---------------- ImGuiFileDialog ----------------
set(IMGUIFILEDIALOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../extern/ImGuiFileDialog)

add_library(imgui_file_dialog STATIC
        ${IMGUIFILEDIALOG_DIR}/ImGuiFileDialog.cpp
)

target_include_directories(imgui_file_dialog PUBLIC
        ${IMGUIFILEDIALOG_DIR}
        ${IMGUI_DIR}      # ImGuiFileDialog needs ImGui headers
)

# ---------------- ImGuiColorTextEdit ----------------
set(TEXTEDITOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../extern/ImGuiColorTextEdit)

add_library(imgui_text_editor STATIC
        ${TEXTEDITOR_DIR}/TextEditor.cpp
        /Users/christiancruz/CLionProjects/NebulaEngine/extern/ImGuiColorTextEdit/TextEditor.cpp
)

target_include_directories(imgui_text_editor PUBLIC
        ${TEXTEDITOR_DIR}
        ${IMGUI_DIR}
)


# ---------------- Link Libraries ----------------
target_link_libraries(Nebula
        PUBLIC
        glad
        glfw
        imgui
        imguizmo
        imgui_file_dialog
        imgui_text_editor
        stdc++
        assimp
        "-framework Metal"
        "-framework Foundation"
        "-framework QuartzCore"
        "-framework AppKit" objc
)