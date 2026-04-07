//
// Created by Christian Cruz on 4/5/26.
//

#ifndef ENTITYHANDLE_H
#define ENTITYHANDLE_H

#include "Core.h"
#include "entt/entt.hpp"

namespace Nebula {
    class EntityHandle {
        public:
            EntityHandle() = default;
            ~EntityHandle() = default;

            EntityHandle(entt::entity entity, entt::registry* registry)
                : m_EntityID(entity), m_Registry(registry) {
            }

        template<typename T, typename... Args>
            T& GetOrAddComponent(Args&&... args) const {
                // If the component already exists, return it.
                if (HasComponent<T>()) {
                    return GetComponent<T>();
                }

                // Insert the component. Some entt versions make emplace<T> return void,
                // so we don't rely on its return value. Instead we call get<T>() after emplacing.
                m_Registry->emplace<T>(m_EntityID, std::forward<Args>(args)...);

                // Now return the reference to the component we just created.
                return m_Registry->get<T>(m_EntityID);
            }

            template<typename T>
            T& GetComponent() const {
                return m_Registry->get<T>(m_EntityID);
            }

            template<typename T>
            bool HasComponent() const {
                return m_Registry->all_of<T>(m_EntityID);
            }

            template<typename T>
            void RemoveComponent() const {
                if (!m_Registry) {
                    printf("Registry is null");
                    return;
                }

                if (!m_Registry->valid(m_EntityID)) {
                    printf("Entity is not valid");
                    return;
                }

                if (m_Registry->all_of<T>(m_EntityID)) {
                    m_Registry->remove<T>(m_EntityID);
                }
            }

            entt::entity GetEnttID() const { return m_EntityID; }
            bool IsValid() const { return m_EntityID != entt::null; }


        private:
            entt::entity m_EntityID{ entt::null };
            // using a raw pointer because the registry is owned by the scene
            // only the scene should be able to delete the registry
            entt::registry* m_Registry = nullptr;
        };
} // Nebula
#endif //ENTITYHANDLE_H
