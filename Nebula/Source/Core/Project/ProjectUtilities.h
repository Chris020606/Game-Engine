//
// Created by Christian Cruz on 4/5/26.
//

#ifndef PROJECTUTILITIES_H
#define PROJECTUTILITIES_H

#include <filesystem>


namespace Nebula {

	// Finds all files with a given extension in a directory (non-recursive).
	// Returns an empty vector if the folder is invalid or no matching files are found.
    inline std::vector<std::filesystem::path> FindFilesInFolder(const std::filesystem::path& folder, const std::string& extension) {
        std::vector<std::filesystem::path> result;
		if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
            std::print("Folder does not exists or is not a directory {0}", folder.string());
			return result;
		}
		for (const auto& entry : std::filesystem::directory_iterator(folder)) {
			if (entry.is_regular_file() && entry.path().extension() == extension) {
				result.push_back(entry.path());
			}
		}
		return result;
    }

	// Checks whether the given file resides directly in the specified folder.
	// Returns false if either path is invalid.
	inline bool IsFileInFolder(const std::filesystem::path& filepath, const std::filesystem::path& folderpath) {
		if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath)) {
			std::print("Invalid filepath passed: {0}", filepath.string());
			return false;
		}
		if (!std::filesystem::exists(folderpath) || !std::filesystem::is_directory(folderpath)) {
			std::print("Invalid folderpath passed: {0}", folderpath.string());
			return false;
		}
		return filepath.parent_path() == folderpath;
	}


	// Appends a new extension string to the given path without removing the existing one.
	// Example: "asset/cube.fbx" + ".nbmeta" -> "asset/cube.fbx.nbmeta"
	inline std::filesystem::path AppendExtension(const std::filesystem::path& path, const std::string& extension) {
		return std::filesystem::path(path.string() + extension);
	}

	// Removes the last extension from a path while preserving its directory.
	// Example: "asset/cube.fbx.nbmeta" -> "asset/cube.fbx"
	inline std::filesystem::path StripExtension(const std::filesystem::path& path) {
		std::filesystem::path assetFilename = path.stem();		// strips 1 extension
		std::filesystem::path pathToAsset = path.parent_path(); // keeps the path to the file
		return pathToAsset / assetFilename;
	}

} // Nebula

#endif //PROJECTUTILITIES_H
