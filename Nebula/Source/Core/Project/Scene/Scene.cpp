//
// Created by Christian Cruz on 4/5/26.
//

#include "Scene.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <glm/glm.hpp>

namespace Nebula {

    EntityHandle Scene::CreateEntity(const std::string& name) {
        entt::entity entityID = m_Registry->create();
        EntityHandle entity(entityID, m_Registry);
        entity.GetOrAddComponent<IDComponent>();
        entity.GetOrAddComponent<TagComponent>(name);
        return entity;
    }

    EntityHandle Scene::CreateEntityWithGuid(ID guid, const std::string& name) {
        entt::entity entityID = m_Registry->create();
        EntityHandle entity(entityID, m_Registry);
        entity.GetOrAddComponent<IDComponent>(guid);
        entity.GetOrAddComponent<TagComponent>(name);
        return entity;
    }

    void Scene::DestroyEntity(EntityHandle entity) {
        m_Registry->destroy(entity.GetEnttID());
    }


    void Scene::OnStart() {
        // TODO Create the main camera
        // Here is where create stuff for the scene
    }


    void Scene::OnUpdate(float ts) {
        // Update Scene
        // Draw Scene

    }


    void Scene::OnShutdown() {
        // Delete all memory of the scene
    }

    EntityHandle Scene::GetPrimaryCameraEntity() {
        auto view = m_Registry->view<CameraComponent>();
        for (auto entity : view) {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.is_main)
                return EntityHandle{entity, m_Registry};
        }
        return {};
    }


    Ref<Scene> Scene::Copy(Ref<Scene> other) {
        auto newScene = std::make_shared<Scene>();

        newScene->id = other->id;
        newScene->name = other->name;
        newScene->skyboxid = other->skyboxid;
        newScene->skyboxName = other->skyboxName;

        auto* src = other->m_Registry;
        auto* dst = newScene->m_Registry;

        auto view = src->view<IDComponent, TagComponent>();

        for (auto [srcEntity, id, tag] : view.each()) {
            entt::entity dstEntity = newScene->CreateEntityWithGuid(id.id, tag.Tag).GetEnttID();
            // IDComponent and TagComponent already copied on CreateEntityWithGuid

            if (src->any_of<TransformComponent>(srcEntity)) {
                dst->emplace_or_replace<TransformComponent>(dstEntity, src->get<TransformComponent>(srcEntity));
            }
            if (src->any_of<CameraComponent>(srcEntity)) {
                dst->emplace_or_replace<CameraComponent>(dstEntity, src->get<CameraComponent>(srcEntity));
            }
            if (src->any_of<MeshComponent>(srcEntity)) {
                dst->emplace_or_replace<MeshComponent>(dstEntity, src->get<MeshComponent>(srcEntity));
            }
            if (src->any_of<MaterialComponent>(srcEntity)) {
                dst->emplace_or_replace<MaterialComponent>(dstEntity, src->get<MaterialComponent>(srcEntity));
            }
        }

        return newScene;
    }
    bool SaveSceneFile(const std::filesystem::path& scenepath,
                   Ref<const Scene> scene)
    {
        if (!(scenepath.has_extension() &&
              scenepath.extension() == SCENE_FILE_EXTENSION))
            return false;

        // 🔥 THIS WAS THE BUG
        std::filesystem::create_directories(scenepath.parent_path());

    nlohmann::json root;

    root["SceneGuid"]  = static_cast<uint64_t>(scene->id);
    root["SceneName"]  = scene->name;
    root["SkyboxGuid"] = static_cast<uint64_t>(scene->skyboxid);
    root["SkyboxName"] = scene->skyboxName;

    auto& entities = root["Entities"] = nlohmann::json::array();

    for (auto& e : scene->GetRegistry()->view<entt::entity>()) {
        EntityHandle entity{ e, scene->GetRegistry() };
        nlohmann::json ent;

        if (entity.HasComponent<TagComponent>())
            ent["TagComponent"] = entity.GetComponent<TagComponent>().Tag;

        if (entity.HasComponent<IDComponent>())
            ent["IDComponent"] = (uint64_t)entity.GetComponent<IDComponent>().id;

        if (entity.HasComponent<TransformComponent>()) {
            auto& tc = entity.GetComponent<TransformComponent>();
            ent["TransformComponent"]["Translation"] = { tc.GetTranslation().x, tc.GetTranslation().y, tc.GetTranslation().z };
            ent["TransformComponent"]["Rotation"]    = { tc.GetRotation().x, tc.GetRotation().y, tc.GetRotation().z };
            ent["TransformComponent"]["Scale"]       = { tc.GetScale().x, tc.GetScale().y, tc.GetScale().z };
        }

        if (entity.HasComponent<CameraComponent>()) {
            auto& cc = entity.GetComponent<CameraComponent>();
            ent["CameraComponent"]["IsMain"] = cc.is_main;
            ent["CameraComponent"]["Fov"]    = cc.m_Camera.GetFov();
        }

        if (entity.HasComponent<MeshComponent>()) {
            auto& mc = entity.GetComponent<MeshComponent>();
            ent["MeshComponent"]["SourceName"] = mc.sourceName;
            ent["MeshComponent"]["MeshGuid"]   = static_cast<uint64_t>(mc.guid);
        }

        if (entity.HasComponent<MaterialComponent>()) {
            auto& mc = entity.GetComponent<MaterialComponent>();
            ent["MaterialComponent"]["Specular"] =
                { mc.specular.x, mc.specular.y, mc.specular.z};
            ent["MaterialComponent"]["Color"] =
                { mc.color.x, mc.color.y, mc.color.z };
            ent["MaterialComponent"]["Shininess"] =
                { mc.shininess };
            ent["MaterialComponent"]["Diffuse"] =
                { mc.diffuse.x, mc.diffuse.y, mc.diffuse.z };
            ent["MaterialComponent"]["Ambient"] =
                { mc.ambient.x, mc.ambient.y, mc.ambient.z };
        }

        if (entity.HasComponent<LightComponent>()) {
            auto& lc = entity.GetComponent<LightComponent>();
            ent["LightComponent"]["Position"]=
                { lc.Position.x, lc.Position.y, lc.Position.z };
            ent["LightComponent"]["Ambient"]=
                { lc.Ambient.x, lc.Ambient.y, lc.Ambient.z };
            ent["LightComponent"]["Diffuse"]=
                { lc.Diffuse.x, lc.Diffuse.y, lc.Diffuse.z };
            ent["LightComponent"]["Specular"]=
                { lc.Specular.x, lc.Specular.y, lc.Specular.z };
        }

        entities.push_back(ent);
    }

    std::ofstream fout(scenepath);
    if (!fout) {
        std::print("SaveSceneFile: failed to open file '{}'.", scenepath.string());
        return false;
    }

    fout << root.dump(4);
    std::print("SaveSceneFile: successfully saved scene to {}", scenepath.string());
    return true;
}
    std::shared_ptr<Scene> LoadSceneFile(const std::filesystem::path& scenepath) {
    if (!(std::filesystem::exists(scenepath) &&
          std::filesystem::is_regular_file(scenepath) &&
          scenepath.has_extension() &&
          scenepath.extension() == SCENE_FILE_EXTENSION))
    {
        std::print("LoadSceneFile: invalid or missing scene file: {}", scenepath.string());
        return nullptr;
    }

    nlohmann::json root;
    try {
        std::ifstream in(scenepath);
        in >> root;
    }
    catch (const std::exception& e) {
        std::print("LoadSceneFile: JSON parse error while reading {}: {}", scenepath.string(), e.what());
        return nullptr;
    }

    auto scene = std::make_shared<Scene>();

    auto get = [&](auto& j, auto key, auto def) {
        using T = std::decay_t<decltype(def)>;

        if (j.contains(key))
            return j[key].template get<T>();

        return def;
    };


    auto getFloat = [&](nlohmann::json& j, auto key) {
        if (j.contains(key)) {
            return j[key][0];
        }
    };

    auto getVec3 = [&](nlohmann::json& j, const char* key) {
        if (!j.contains(key) || !j[key].is_array() || j[key].size() < 3)
            return glm::vec3(0.f);
        return glm::vec3(j[key][0], j[key][1], j[key][2]);
    };

    auto getVec4 = [&](nlohmann::json& j, const char* key) {
        if (!j.contains(key) || !j[key].is_array() || j[key].size() < 4)
            return glm::vec4(0.f);
        return glm::vec4(j[key][0], j[key][1], j[key][2], j[key][3]);
    };

    scene->id       = static_cast<ID>(get(root, "Sceneid", uint64_t(0)));
    scene->name       = get(root, "SceneName", std::string("Untitled Scene"));
    scene->skyboxid = static_cast<ID>(get(root, "Skyboxid", uint64_t(0)));
    scene->skyboxName = get(root, "SkyboxName", std::string(""));

    if (root.contains("Entities") && root["Entities"].is_array()) {
        for (auto& entityNode : root["Entities"]) {

            auto name = get(entityNode, "TagComponent", std::string("Unnamed Entity"));
            auto guid = static_cast<ID>(get(entityNode, "IDComponent", (uint64_t)ID{}));

            EntityHandle entity = scene->CreateEntityWithGuid(guid, name);

            if (entityNode.contains("TransformComponent")) {
                auto& tc = entity.GetOrAddComponent<TransformComponent>();
                auto& tnode = entityNode["TransformComponent"];
                tc.SetTranslation(getVec3(tnode, "Translation"));
                tc.SetRotation   (getVec3(tnode, "Rotation"));
                tc.SetScale      (getVec3(tnode, "Scale"));
            }

            if (entityNode.contains("CameraComponent")) {
                auto& cc = entity.GetOrAddComponent<CameraComponent>();
                auto& cnode = entityNode["CameraComponent"];
                cc.is_main = get(cnode, "IsMain", false);
                cc.fov = get(cnode, "Fov", 60.0f);
            }

            if (entityNode.contains("MeshComponent")) {
                auto& mc = entity.GetOrAddComponent<MeshComponent>();
                auto& mnode = entityNode["MeshComponent"];
                mc.sourceName = get(mnode, "SourceName", std::string(""));
                mc.guid       = static_cast<ID>(get(mnode, "MeshGuid", uint64_t(0)));
            }

            if (entityNode.contains("MaterialComponent")) {
                auto& mc = entity.GetOrAddComponent<MaterialComponent>();
                auto& mnode = entityNode["MaterialComponent"];
                mc.specular = getVec3(mnode, "Emission");
                mc.color    = getVec3(mnode, "Color");
                mc.ambient  = getVec3(mnode, "Ambient");
                mc.diffuse  = getVec3(mnode, "Diffuse");
                mc.shininess= getFloat(mnode, "Shininess");
            }

            if (entityNode.contains("LightComponent")) {
                auto& lc = entity.GetOrAddComponent<LightComponent>();
                auto& lnode = entityNode["LightComponent"];
                lc.Ambient = getVec3(lnode, "Ambient");
                lc.Diffuse = getVec3(lnode, "Diffuse");
                lc.Specular = getVec3(lnode, "Specular");
                lc.Position = getVec3(lnode, "Position");
            }
        }
    }

    std::print("LoadSceneFile: successfully loaded scene from {}", scenepath.string());
    return scene;
}
} // Nebula