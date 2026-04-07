project(App)

add_executable(App
        main.cpp
        App.cpp
        App.h
        Editor/EditorLayer.cpp
        Editor/EditorLayer.h
        Editor/ThemeManager.cpp
        Editor/ThemeManager.h

)


target_link_libraries(App
        PRIVATE Nebula
        imgui
        imguizmo
        imgui_file_dialog

)