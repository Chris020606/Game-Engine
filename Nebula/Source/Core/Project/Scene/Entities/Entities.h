//
// Created by Christian Cruz on 4/5/26.
//

#ifndef ENTITIES_H
#define ENTITIES_H

#include "Core.h"
#include "entt/entt.hpp"

namespace Nebula {

    class Entities {
        public:
            Entities(entt::registry& registry) : registry(registry) {}
            ~Entities() {}
        private:
            entt::registry& registry;
    };

} // Nebula


#endif //ENTITIES_H
