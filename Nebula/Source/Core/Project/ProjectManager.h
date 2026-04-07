//
// Created by Christian Cruz on 3/18/26.
//

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <filesystem>
#include "Core.h"
#include "Scene/SceneManager.h"
#include "Assets/AssectsManager.h"
#include "ID.h"


namespace Nebula {
    #define PROJECT_FILE_EXTENSION ".nbproj"

    struct ProjectFile {
        ID SceneID = ID::INVALID;
        ProjectFile(ID sceneID = ID::INVALID) : SceneID(sceneID) {}
    };

    bool SaveProjectFile(const std::filesystem::path& projectFilepath, const ProjectFile& projectFile);

    inline std::filesystem::path ComposeProjectFilepath(const std::filesystem::path& folderpath) {
        if (folderpath.empty()) {
             printf("ComposeProjectFilepath: folderpath is empty");
            return std::filesystem::path{};
        }
        if (!std::filesystem::exists(folderpath)) {
            std::print("ComposeProjectFilepath: folderpath does not exist: {}", folderpath.string());
            return std::filesystem::path{};
        }
        std::string folderName = folderpath.filename().string();
        if (folderName.empty()) {
            std::print("ComposeProjectFilepath: could not extract folder name from: {}", folderpath.string());
            return std::filesystem::path{};
        }
        return folderpath / (folderName + PROJECT_FILE_EXTENSION);
    }

    class ProjectManager {
        public:
        bool NewProject(const std::filesystem::path& folderPath);
        bool OpenProject(const std::filesystem::path& projectfilePath);
        bool SaveProject();
        void CloseProject();

        bool ProjectIsOpen() const { return !m_ProjectFolder.empty(); }
        Ref<SceneManager> GetSceneManager() const { return m_SceneManager; }
        Ref<AssetManager> GetAssetManager() const { return m_AssetManager; }

        void SetBootSceneGuid(ID id) { m_ProjectFile.SceneID = id; }
        ID GetBootSceneGuid() const { return m_ProjectFile.SceneID; }
        bool IsBootScene(ID id) { return m_ProjectFile.SceneID== id; }

        std::string GetProjectName() { return m_ProjectFolder.filename().string(); }
        std::filesystem::path GetProjectFolder() { return m_ProjectFolder; }
        private:
            /// Filesystem path to the current project folder (where .nbproj lives).
            std::filesystem::path m_ProjectFolder;
            ProjectFile m_ProjectFile;

            Ref<SceneManager> m_SceneManager = nullptr;
            Ref<AssetManager> m_AssetManager = nullptr;
    };
}

#endif //PROJECTMANAGER_H
