//
// Created by Christian Cruz on 3/18/26.
//

#include "ProjectManager.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Nebula {
        	// PROJECT FILE ------------------------------------------------------------------------------
	bool SaveProjectFile(const std::filesystem::path& projectFilepath, const ProjectFile& projectFile) {
		if (!(projectFilepath.has_extension() && projectFilepath.extension() == PROJECT_FILE_EXTENSION)) {
			//LOG_ENGINE_WARN("SaveProjectFile: invalid file extension '{}'.", projectFilepath.string());
			return false;
		}

		if (!std::filesystem::exists(projectFilepath.parent_path())) {
			//LOG_ENGINE_WARN("SaveProjectFile: parent directory '{}' does not exist.", projectFilepath.parent_path().string());
			return false;
		}

		json j;

		// Basic project data
		j["bootSceneGuid"] = (uint64_t)projectFile.SceneID;

		// Runtime render settings
		j["RuntimeRenderSettings"] = json::object();
		projectFile.runtimeRenderSettings.SerializeToJson(j["RuntimeRenderSettings"]);

		// Write file
		std::ofstream fout(projectFilepath);
		if (!fout.is_open()) {
			//LOG_ENGINE_ERROR("SaveProjectFile: could not open {} for writing - permissions or path invalid", projectFilepath.string());
			return false;
		}

		fout << j.dump(4); // pretty-print

		//LOG_ENGINE_INFO("SaveProjectFile: wrote project data into {}", projectFilepath.string());
		return true;
	}


	std::optional<ProjectFile> LoadProjectFile(const std::filesystem::path& projectFilepath) {
		if (!(std::filesystem::exists(projectFilepath) &&
			  std::filesystem::is_regular_file(projectFilepath) &&
			  projectFilepath.has_extension() &&
			  projectFilepath.extension() == PROJECT_FILE_EXTENSION))
		{
			//LOG_ENGINE_WARN("LoadProjectFile: invalid or missing project file: {}", projectFilepath.string());
			return std::nullopt;
		}

		try {
			std::ifstream fin(projectFilepath);
			if (!fin.is_open()) {
				//LOG_ENGINE_ERROR("LoadProjectFile: failed to open {}", projectFilepath.string());
				return std::nullopt;
			}

			json j;
			fin >> j;

			ProjectFile projectFile;

			projectFile.bootSceneGuid = static_cast<GUIDs>(j.at("bootSceneGuid").get<uint64_t>());

			// Deserialize runtime settings
			if (j.contains("RuntimeRenderSettings")) {
				projectFile.runtimeRenderSettings.DeserializeFromJson(
					j["RuntimeRenderSettings"]
				);
			}

			//LOG_ENGINE_INFO("LoadProjectFile: successfully loaded project file from {}", projectFilepath.string());
			return projectFile;
		}
		catch (const std::exception& e) {
			//LOG_ENGINE_ERROR("LoadProjectFile: JSON parse error while reading {}: {}", projectFilepath.string(), e.what())
			//;
			std::cout << "LoadProjectFile: JSON parse error while reading "
		  << projectFilepath.string()
		  << ": " << e.what()
		  << std::endl;
		   return std::nullopt;
		}
	}




	// PROJECT MANAGER ----------------------------------------------------------------------------
	bool ProjectManager::NewProject(const std::filesystem::path& folderpath) {
		if (std::filesystem::exists(folderpath)) {
			//LOG_ENGINE_WARN("NewProject: folder already exists at {0}", folderpath.string());
			return false;
		}

		if (!std::filesystem::create_directory(folderpath)) {
			//LOG_ENGINE_ERROR("NewProject: failed to create directory at {0}", folderpath.string());
			return false;
		}

		m_ProjectFolder = folderpath;
		m_ProjectFile = ProjectFile{};

		m_AssetManager = std::make_shared<AssetManager>();
		m_SceneManager = std::make_shared<SceneManager>();

		if (!SaveProject()) {
			//LOG_ENGINE_ERROR("NewProject: failed to save initial project file at {0}", folderpath.string());
			return false;
		}

		//LOG_ENGINE_INFO("NewProject: successfully created project at {0}", folderpath.string());
		return true;
	}


	bool ProjectManager::OpenProject(const std::filesystem::path& projectfilePath) {
		if (!std::filesystem::exists(projectfilePath) ||
			!std::filesystem::is_regular_file(projectfilePath) ||
			projectfilePath.extension() != PROJECT_FILE_EXTENSION)
		{
			//LOG_ENGINE_ERROR("OpenProject: invalid .lrproj file selected: {}", projectfilePath.string());
			return false;
		}

		auto folderpath = projectfilePath.parent_path();
		m_ProjectFolder = folderpath;

		std::filesystem::path projectFilepath = ComposeProjectFilepath(folderpath);
		std::cout << "Opening project file: " << projectFilepath << std::endl;
		auto projectFile = LoadProjectFile(projectFilepath);
		m_ProjectFile = projectFile.value_or(ProjectFile{});

		if (!projectFile) {
			//LOG_ENGINE_WARN("OpenProject: failed to deserialize project file at {}", projectFilepath.string());
		}

		m_AssetManager = std::make_shared<AssetManager>();
		m_SceneManager = std::make_shared<SceneManager>();

		m_AssetManager->LoadAssetPoolFromFolder(folderpath);
		m_SceneManager->LoadScenesFromFolder(folderpath);

		if (m_ProjectFile.bootSceneGuid != GUIDs::INVALID) {
			m_SceneManager->SetOpenSceneGuid(m_ProjectFile.bootSceneGuid);
		}

		//LOG_ENGINE_INFO("OpenProject: successfully opened project at {}", folderpath.string());
		return true;
	}


	bool ProjectManager::SaveProject() {
		if (!ProjectIsOpen()) {
			//LOG_ENGINE_WARN("SaveProject: no project is currently open");
			return false;
		}

		bool success = true;
		std::filesystem::path projectFilepath = ComposeProjectFilepath(m_ProjectFolder);

		m_SceneManager->SaveScenesToFolder(m_ProjectFolder);
		m_AssetManager->SaveAssetPoolToFolder(m_ProjectFolder);

		if (!SaveProjectFile(projectFilepath, m_ProjectFile)) {
			//LOG_ENGINE_ERROR("SaveProject: failed to serialize project file at {0}", projectFilepath.string());
			success = false;
		}// else { LOG_ENGINE_INFO("SaveProject: wrote project data into {0}", projectFilepath.string()); }

		// m_SceneManager->SaveScenesToFolder(m_ProjectFolder);
		// m_AssetManager->SaveAssetPoolToFolder(m_ProjectFolder);
		return success;
	}

	void ProjectManager::CloseProject() {
		if (!ProjectIsOpen()) {
			//LOG_ENGINE_WARN("CloseProject: no project is currently open");
			return;
		}

		//LOG_ENGINE_INFO("CloseProject: closing project at {0}", m_ProjectFolder.string());

		m_ProjectFolder.clear();
		m_ProjectFile = ProjectFile{};
		m_AssetManager = nullptr;
		m_SceneManager = nullptr;

		//LOG_ENGINE_INFO("CloseProject: project closed successfully");
	}
}