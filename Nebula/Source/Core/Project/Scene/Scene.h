//
// Created by Christian Cruz on 4/5/26.
//

#ifndef SCENE_H
#define SCENE_H

#include "Core.h"
#include <filesystem>
#include "Entities/EntityHandle.h"
#include "Components/Components.h"


namespace Nebula {

#define SCENE_FILE_EXTENSION ".nbscn"

class Scene {
    public:
        explicit Scene() {m_Registry = new entt::registry();}
        ~Scene() {delete m_Registry;}

        EntityHandle CreateEntity(const std:: string& name = "Empty Entity");
        EntityHandle CreateEntityWithGuid(ID id, const std::string& name);

        void DestroyEntity(EntityHandle entity);
        void OnStart (); // TODO When starting a scene create a camera
        void OnUpdate(float ts);
        void OnShutdown ();
        static Ref<Scene> Copy(Ref<Scene> other);
        entt::registry* GetRegistry() const {return m_Registry; }
        EntityHandle GetPrimaryCameraEntity();

        ID id;
        std::string name;

        ID		skyboxid;
        std::string skyboxName;

    private:
        friend bool SaveSceneFile(const std::filesystem::path& scenepath, Ref<const Scene> scene);
        friend Ref<Scene> LoadSceneFile(const std::filesystem::path& scenepath);
        entt::registry* m_Registry;

};

    bool SaveSceneFile(const std::filesystem::path& scenepath, Ref<const Scene> scene);
    Ref<Scene> LoadSceneFile(const std::filesystem::path& scenepath);

} // Nebula

#endif //SCENE_H
