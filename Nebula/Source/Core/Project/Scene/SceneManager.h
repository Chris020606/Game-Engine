//
// Created by Christian Cruz on 4/5/26.
//

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "Core.h"
#include "ID.h"
#include "Scene.h"
#include "Project/ProjectUtilities.h"

namespace Nebula {

class SceneManager {
    public:
    SceneManager();
    ~SceneManager();

    ID CreateScene(const std::string& name = "Empty Scene");
    void DeleteScene(ID id);

    void OpenSceneGUID(ID id);

    Ref<Scene> GetOpenedScene(ID id) const;

    bool SetOpenSceneGuid(ID id);
    ID GetOpenSceneGuid() const;

    Ref<Scene> GetOpenScene() const;

    void EnterRuntimeSimulation();
    void ExitRuntimeSimulation();

    /// Saves all loaded scenes as `.lrscene` files into the specified folder.
    /// - Overwrites existing files with the same GUID.
    /// - Attempts to remove any orphaned scene files not backed by in-memory scenes.
    /// - Logs warnings on failures.
    /// - Does not throw; always tries to complete all work.
    void SaveScenesToFolder(const std::filesystem::path& folderpath) const;

    /// Loads all `.lrscene` files from the specified folder into memory.
    /// - Skips and logs any files that fail to deserialize.
    /// - Does not throw; continues loading remaining scenes.
    void LoadScenesFromFolder(const std::filesystem::path& folderpath);

    /// Iterators to enable range-based for loops over SceneManager:
    /// e.g. `for (auto& scene : SceneManager) { ... }`
    auto begin()       { return m_ActiveScenes->begin(); }
    auto end()         { return m_ActiveScenes->end(); }


    /// Finds and returns a shared pointer to the Scene with the given GUID.
    /// Returns nullptr if the GUID is invalid or if no matching Scene is found.
    inline Ref<Scene> find(ID id) const {
        if (id == ID::INVALID) {
            return nullptr;
        }
        if (auto it = m_ActiveScenes->find(id); it != m_ActiveScenes->end()){
            return it->second;
        }
        return nullptr;
    }
private:
    ID m_OpenSceneGuid = ID::INVALID;
    ID m_OpenSceneGuidCache = ID::INVALID;

    bool m_InRuntimeSimulation = false;
    std::unordered_map<ID, Ref<Scene>> m_Scenes;
    std::unordered_map<ID, Ref<Scene>> m_RuntimeSimulationScenes;

    // m_ActiveScenes are either m_Scenes || m_RuntimeSimulationScenes
    std::unordered_map<ID, Ref<Scene>>* m_ActiveScenes = &m_Scenes;


    // Utilities ========================
    inline static ID ExtractGuidFromScenepath(const std::filesystem::path& scenepath) {
        std::string stem = scenepath.filename().stem().string(); // remove extension
        try {
            return ID(std::stoull(stem)); // convert stem to unsigned long long GUID
        }
        catch (const std::exception&) {
            std::print("ExtractGuid: filename \"{0}\" isn't a valid unsigned integer - skipping", scenepath.string());
            return ID::INVALID;
        }
    }

    inline static std::filesystem::path ComposeScenepathFromGuid(const std::filesystem::path& folderpath, ID id) {
        return folderpath / (std::to_string(static_cast<uint64_t>(id)) + SCENE_FILE_EXTENSION);
    }

};

} // Nebula

#endif //SCENEMANAGER_H
