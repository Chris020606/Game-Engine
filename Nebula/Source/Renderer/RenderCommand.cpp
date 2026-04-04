//
// Created by Christian Cruz on 2/12/26.
//

#include "RenderCommand.h"
#include "RendererAPI.h"
#include "/Users/christiancruz/CLionProjects/NebulaEngine/Nebula/Source/Platform/Mac/Metal/MetalAPI/MetalRendererAPI.h"


namespace Nebula {
    Ref<RendererAPI> RenderCommand::s_RendererAPI = nullptr;
} // Nebula