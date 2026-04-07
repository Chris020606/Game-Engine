//
// Created by Christian Cruz on 4/5/26.
//

#include "SceneManager.h"

namespace Nebula {

    SceneManager::SceneManager() {

	}

	SceneManager::~SceneManager() {

	}


	ID SceneManager::CreateScene(const std::string& name) {
		if (m_InRuntimeSimulation) {
			std::print("CreateScene: Cannot create scenes while in runtime simulation");
			return ID::INVALID;
		}

		auto scene = std::make_shared<Scene>();
		scene->name = name;
		(*m_ActiveScenes)[scene->id] = scene;
		std::print("CreateScene: created new scene \"{0}\" with GUID {1}", name, (uint64_t)scene->id);
		return scene->id;
	}

	void SceneManager::DeleteScene(ID id) {
		if (m_InRuntimeSimulation) {
			std::print("DeleteScene: Cannot delete scenes while in runtime simulation");
			return;
		}

		if (m_OpenSceneGuid == id) {
			m_OpenSceneGuid = ID::INVALID;
			std::print("DeleteScene: closed open scene (GUID {0}) because it was deleted", (uint64_t)id);
		}

		size_t removed = m_ActiveScenes->erase(id);
		if (removed == 0) {
			std::print("DeleteScene: no scene found with GUID {0}; nothing deleted", (uint64_t)id);
		} else {
			std::print("DeleteScene: successfully removed scene with GUID {0}", (uint64_t)id);
		}
	}

	bool SceneManager::SetOpenSceneGuid(ID id) {
		auto scene = find(id);
		if (!scene && id != ID::INVALID) { // allow setting to LR_GUID::INVALID (no scene selected)
			std::print("SetOpenScene: cannot open scene; no scene registered with GUID {0}", (uint64_t)id);
			return false;
		}

		m_OpenSceneGuid = id;
		std::print("SetOpenScene: now tracking scene with GUID {0} as the active scene", (uint64_t)id);
		return true;
	}

	ID SceneManager::GetOpenSceneGuid() const {
		return m_OpenSceneGuid;
	}

	Ref<Scene> SceneManager::GetOpenScene() const {
		return find(m_OpenSceneGuid);
	}

	Ref<Scene> SceneManager::GetOpenedScene(ID id) const
	{
		return find(id);
	}

	void SceneManager::EnterRuntimeSimulation() {
		if (m_InRuntimeSimulation) { return; }

		m_InRuntimeSimulation = true;

		m_RuntimeSimulationScenes.clear(); // remove all data
		for (const auto& [guid, scene] : m_Scenes) {
			Ref<Scene> sceneCopy = Scene::Copy(scene);
			m_RuntimeSimulationScenes.emplace(guid, sceneCopy);
		}

		m_OpenSceneGuidCache = m_OpenSceneGuid; // cache the currently open scene id
		m_ActiveScenes = &m_RuntimeSimulationScenes;
	}


	void SceneManager::ExitRuntimeSimulation() {
		if (!m_InRuntimeSimulation) { return; }

		m_InRuntimeSimulation = false;
		m_RuntimeSimulationScenes.clear(); // remove all data
		m_OpenSceneGuid = m_OpenSceneGuidCache; // recover the scene id
		m_ActiveScenes = &m_Scenes;
	}


	void SceneManager::SaveScenesToFolder(const std::filesystem::path& folderpath) const {
		// Delete orphaned .lrscene files
		for (const auto& scenepath : FindFilesInFolder(folderpath, SCENE_FILE_EXTENSION)) {
			ID guid = ExtractGuidFromScenepath(scenepath);
			if (guid == ID::INVALID) {
				std::print("SaveScenesToFolder: invalid GUID in filename \"{0}\"; skipping orphaned file", scenepath.string());
				continue;
			}

			if (!find(guid)) {
				std::print("SaveScenesToFolder: deleting orphaned scene file \"{0}\" (GUID {1})", scenepath.string(), (uint64_t)guid);
				std::filesystem::remove(scenepath);
			}
		}

		// Serialize in-memory scenes
		for (const auto& [guid, scene] : m_Scenes) { // always save the real scenes
			std::filesystem::path scenepath = ComposeScenepathFromGuid(folderpath, guid);
			if (!SaveSceneFile(scenepath, scene)) {
				std::print("SaveScenesToFolder: failed to serialize scene GUID {0} to \"{1}\"", (uint64_t)guid, scenepath.string());
			} else {
				std::print("SaveScenesToFolder: successfully saved scene GUID {0} to \"{1}\"", (uint64_t)guid, scenepath.string());
			}
		}
	}

	void SceneManager::LoadScenesFromFolder(const std::filesystem::path& folderpath) {
		for (const auto& scenepath : FindFilesInFolder(folderpath, SCENE_FILE_EXTENSION)) {
			auto scene = LoadSceneFile(scenepath);
			if (!scene) {
				std::print("LoadScenesFromFolder: failed to deserialize scene file \"{0}\"; skipping", scenepath.string());
				continue;
			}
			ID guid = scene->id;
			m_Scenes[guid] = scene;
			std::print("LoadScenesFromFolder: loaded scene \"{0}\" with GUID {1}", scenepath.string(), (uint64_t)guid);
		}
	}
} // Nebula