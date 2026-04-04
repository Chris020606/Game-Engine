//
// Created by Christian Cruz on 1/6/26.
//

#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H
#include <string>

#include "Core.h"


namespace Nebula {

class ComputeShader {
    public:
    virtual ~ComputeShader() = default;
    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    virtual void Dispatch(uint32_t width, uint32_t height) = 0;

    // Set a float parameter (time, etc.)
    virtual void SetFloat(const std::string& name, float value) = 0;

    // Get output texture for display
    virtual void* GetOutputTexture() const = 0;

    // Resize output texture
    virtual void Resize(uint32_t width, uint32_t height) = 0;


    static Ref<ComputeShader> Create(const std::string& filepath);
    static Ref<ComputeShader> Create(const std::string& filepath, const std::string& functionName);

};

} // Nebula

#endif //COMPUTESHADER_H
