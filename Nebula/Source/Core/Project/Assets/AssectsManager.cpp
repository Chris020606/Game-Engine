//
// Created by Christian Cruz on 4/5/26.
//

#include "AssectsManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <chrono>
#include <fstream>

#include "Project/ProjectUtilities.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Nebula {
    // META FILE ------------------------------------------------------------------------------

	bool SaveMetaFile(const std::filesystem::path& metafilePath, const AssetMetaFile& assetMetafile) {
		if (!(metafilePath.has_extension() && metafilePath.extension() == ASSET_META_FILE_EXTENSION)) {
			std::print("SaveMetaFile: invalid file extension '{}'.", metafilePath.string());
			return false;
		}

		if (!std::filesystem::exists(metafilePath.parent_path())) {
			std::print("SaveMetaFile: parent directory '{}' does not exist.", metafilePath.parent_path().string());
			return false;
		}

		json j;
		j["Guid"] = (uint64_t)assetMetafile.guid;
		j["SourcePath"] = assetMetafile.sourcePath.string();

		std::ofstream fout(metafilePath);
		if (!fout.is_open()) {
			std::print("SaveMetaFile: could not open {} for writing - permissions or path invalid", metafilePath.string());
			return false;
		}

		fout << j.dump(4); // pretty print with indentation

		std::print("SaveMetaFile: wrote metadata for GUID {}", (uint64_t)assetMetafile.guid);
		return true;
	}


	std::optional<AssetMetaFile> LoadMetaFile(const std::filesystem::path& metafilePath) {
		if (!(std::filesystem::exists(metafilePath) &&
			  std::filesystem::is_regular_file(metafilePath) &&
			  metafilePath.has_extension() &&
			  metafilePath.extension() == ASSET_META_FILE_EXTENSION))
		{
			std::print("LoadMetaFile: invalid or missing meta file: {}", metafilePath.string());
			return std::nullopt;
		}

		try {
			std::ifstream fin(metafilePath);
			if (!fin.is_open()) {
				std::print("LoadMetaFile: failed to open {}", metafilePath.string());
				return std::nullopt;
			}

			json j;
			fin >> j;

			AssetMetaFile metafile;
			metafile.guid = (ID)j.at("Guid").get<uint64_t>();
			metafile.sourcePath = std::filesystem::path{ j.at("SourcePath").get<std::string>() };

			std::print("LoadMetaFile: loaded metadata for GUID {}", (uint64_t)metafile.guid);
			return metafile;
		}
		catch (const std::exception& e) {
			std::print("LoadMetaFile: failed to load {}: {}", metafilePath.string(), e.what());
			return std::nullopt;
		}
	}




    // ASSET MANAGER ---------------------------------------------------------------------------
    AssetManager::AssetManager()
        : m_AssetPool(std::make_shared<AssetPool>()) {
    }


    ID AssetManager::ImportAsset(const std::filesystem::path& assetpath) {
        if (!std::filesystem::exists(assetpath) || std::filesystem::is_directory(assetpath)) {
			std::print("ImportAsset: invalid asset path: {0}", assetpath.string());
            return ID::INVALID;
        }

        ID guid;
        if (!LoadAssetFile(assetpath, guid)) {
			std::print("ImportAsset: failed to load asset after saving metafile, removed metafile {0}", assetpath.string());
            return ID::INVALID;
        }

		std::print("ImportAsset: successfully imported asset {0} with GUID {1}", assetpath.string(), (uint64_t)guid);
        return guid;
    }


	void AssetManager::SaveAssetPoolToFolder(const std::filesystem::path& folderpath) const {
		// Delete all existing metafiles which don't have GUID within the asset pool
		for (const auto& metapath : FindFilesInFolder(folderpath, ASSET_META_FILE_EXTENSION)) {
			const auto maybeMetafile = LoadMetaFile(metapath);
			if (!maybeMetafile.has_value()) {
				std::print("SaveAssetPoolToFolder: unable to read metafile {0}", metapath.string());
				continue;
			}
			ID guid = maybeMetafile->guid;
			if (m_AssetPool->Metadata.find(guid) == m_AssetPool->Metadata.end()) {
				std::filesystem::remove(metapath);
				std::print("SaveAssetPoolToFolder: removed stale metafile {0}", metapath.string());
			}
		}

		// Save metafiles for all assets in the asset pool
		for (const auto& [guid, metadataPair] : m_AssetPool->Metadata) {
			const auto& [metadata, metadataExtension] = metadataPair;
			if (metadataExtension && std::filesystem::exists(metadataExtension->sourcePath)) {
				AssetMetaFile metafile{ guid, metadataExtension->sourcePath };

				// save .lrmeta in the project root next to .lrproj file with filename same as the original asset + .lrmeta extension
				auto metapath = folderpath / (metadataExtension->sourcePath.filename().string() + ASSET_META_FILE_EXTENSION);
				if (!SaveMetaFile(metapath, metafile)) {
					std::print("SaveAssetPoolToFolder: failed to save metafile {0}", metapath.string());
				}
				else {
					std::print("SaveAssetPoolToFolder: saved metafile {0}", metapath.string());
				}
			} else {
				std::print("SaveAssetPoolToFolder: asset does not exist {0}", metadataExtension->sourcePath.string());
			}
		}
	}


	void AssetManager::LoadAssetPoolFromFolder(const std::filesystem::path& folderpath) {
		for (const auto& metapath : FindFilesInFolder(folderpath, ASSET_META_FILE_EXTENSION)) {
			auto maybeMetafile = LoadMetaFile(metapath);
			if (!maybeMetafile.has_value()) {
				std::print("LoadAssetPoolFromFolder: failed to load metafile {0}", metapath.string());
				continue;
			}

			auto sourcePath = maybeMetafile->sourcePath;

			// check if the asset exists at the path specified by the .lrmeta file
			if (!std::filesystem::exists(sourcePath)) {
				std::print("LoadAssetPoolFromFolder: missing asset file for metafile {0}", metapath.string());
				continue;
			}

			// if yes then load the asset from that file
			if (!LoadAssetFile(sourcePath, maybeMetafile->guid)) {
				std::print("LoadAssetPoolFromFolder: failed to load asset {0}", sourcePath.string());
				continue;
			}

			std::print("LoadAssetPoolFromFolder: loaded asset {0} with GUID {1}", sourcePath.string(), (uint64_t)maybeMetafile->guid);
		}
	}


	bool AssetManager::LoadAssetFile(const std::filesystem::path& assetpath, ID guid) {
		if (!std::filesystem::exists(assetpath) || !std::filesystem::is_regular_file(assetpath) || !assetpath.has_extension()) {
			std::print("LoadAssetFile: invalid asset path {0}", assetpath.string());
			return false;
		}

		// choose loader based on the extension
		const std::string extension = assetpath.extension().string();
		for (const auto& SUPPORTED_FORMAT : SUPPORTED_MESH_FILE_FORMATS) {
			if (extension == SUPPORTED_FORMAT) {
				std::print("LoadAssetFile: loading mesh {0} for GUID {1}", assetpath.string(), (uint64_t)guid);
				return LoadMesh(assetpath, guid);
			}
		}
		for (const auto& SUPPORTED_FORMAT : SUPPORTED_TEXTURE_FILE_FORMATS) {
			if (extension == SUPPORTED_FORMAT) {
				std::print("LoadAssetFile: loading texture {0} for GUID {1}", assetpath.string(), (uint64_t)guid);
				return LoadTexture(assetpath, guid, 4);
			}
		}

		std::print("LoadAssetFile: unsupported file extension {0}", extension);
		return false;
	}


	bool AssetManager::LoadMesh(const std::filesystem::path& assetpath, ID guid) {
		auto timerStart = std::chrono::high_resolution_clock::now();

		if (!m_AssetPool) {
			std::print("LoadMesh: called without a valid AssetPool for asset {0}", assetpath.string());
			return false;
		}

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(assetpath.string(), aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene) {
			std::print("LoadMesh: failed to load assimp scene from {0} (GUID {1})", assetpath.string(), (uint64_t)guid);
			return false;
		}

		size_t triCount = 0;
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			triCount += scene->mMeshes[i]->mNumFaces;

		std::vector<Triangle>& meshBuffer = m_AssetPool->MeshBuffer;

		auto metadata = std::make_shared<MeshMetadata>();
		metadata->firstTriIdx = meshBuffer.size();
		metadata->TriCount = triCount;

		auto metadataExtension = std::make_shared<MeshMetadataExtension>();
		metadataExtension->sourcePath = assetpath;
		metadataExtension->fileSizeInBytes = std::filesystem::file_size(assetpath);

		meshBuffer.reserve(meshBuffer.size() + triCount);

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* subMesh = scene->mMeshes[i];
			const aiVector3D* verts = subMesh->mVertices;

			for (unsigned int j = 0; j < subMesh->mNumFaces; ++j) {
				const aiFace& face = subMesh->mFaces[j];
				if (face.mNumIndices != 3) continue;

				const auto& idx = face.mIndices;
				meshBuffer.emplace_back(Triangle({
					glm::vec4(verts[idx[0]].x, verts[idx[0]].y, verts[idx[0]].z, 0.0f),
					glm::vec4(verts[idx[1]].x, verts[idx[1]].y, verts[idx[1]].z, 0.0f),
					glm::vec4(verts[idx[2]].x, verts[idx[2]].y, verts[idx[2]].z, 0.0f)
				}));
			}
		}

		m_AssetPool->MarkUpdated(AssetPool::AssetType::MeshBuffer);

		// Build BVH
		BVHAccel bvh(meshBuffer, metadata->firstTriIdx, metadata->TriCount);
		bvh.Build(m_AssetPool->NodeBuffer, m_AssetPool->IndexBuffer, metadata->firstNodeIdx, metadata->nodeCount);

		m_AssetPool->MarkUpdated(AssetPool::AssetType::NodeBuffer);
		m_AssetPool->MarkUpdated(AssetPool::AssetType::IndexBuffer);

		double loadTimeMs = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - timerStart).count();
		metadataExtension->loadTimeMs = loadTimeMs;

		m_AssetPool->Metadata[guid] = { metadata, metadataExtension };
		m_AssetPool->MarkUpdated(AssetPool::AssetType::Metadata);

		std::print("LoadMesh: loaded {0} triangles from {1} (GUID {2}) in {3:.2f} ms",
			triCount, assetpath.string(), (uint64_t)guid, loadTimeMs);
		return true;
	}


	bool AssetManager::LoadTexture(const std::filesystem::path& assetpath, ID guid, int channels) {
		auto timerStart = std::chrono::high_resolution_clock::now();

		if (!m_AssetPool) {
			std::print("LoadTexture: called without a valid AssetPool for asset {0}", assetpath.string());
			return false;
		}

		int width, height, channelsInFile;
		stbi_set_flip_vertically_on_load(1); // OpenGL-style orientation
		unsigned char* data = stbi_load(assetpath.string().c_str(), &width, &height, &channelsInFile, channels);
		if (!data) {
			std::print("LoadTexture: failed to load texture from path={0} (requested channels={1}) for GUID={2}.",
				assetpath.string(), channels, (uint64_t)guid);
			return false;
		}

		const int actualChannels = (channels == 0) ? channelsInFile : channels;
		const size_t totalBytes = width * height * actualChannels;

		std::vector<unsigned char>& textureBuffer = m_AssetPool->TextureBuffer;
		textureBuffer.reserve(textureBuffer.size() + totalBytes);
		textureBuffer.insert(textureBuffer.end(), data, data + totalBytes);
		stbi_image_free(data);

		auto metadata = std::make_shared<TextureMetadata>();
		metadata->texStartIdx = textureBuffer.size() - totalBytes;
		metadata->width = width;
		metadata->height = height;
		metadata->channels = actualChannels;

		auto metadataExt = std::make_shared<TextureMetadataExtension>();
		metadataExt->sourcePath = assetpath;
		metadataExt->fileSizeInBytes = std::filesystem::file_size(assetpath);

		double loadTimeMs = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - timerStart).count();
		metadataExt->loadTimeMs = loadTimeMs;

		m_AssetPool->Metadata[guid] = { metadata, metadataExt };
		m_AssetPool->MarkUpdated(AssetPool::AssetType::TextureBuffer);
		m_AssetPool->MarkUpdated(AssetPool::AssetType::Metadata);

		std::print("LoadTexture: loaded texture {0} (GUID {1}) {2}x{3} with {4} channels in {5:.2f} ms",
			assetpath.string(), (uint64_t)guid, width, height, actualChannels, loadTimeMs);
		return true;
	}
} // Nebula