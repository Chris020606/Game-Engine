#include <metal_stdlib>
using namespace metal;

struct v2f {
    float4 position [[position]];
    float3 normal;
    float2 texCoord;
    float3 color;
};

struct VertexInput {
    float3 position  [[attribute(0)]];
    float3 normal    [[attribute(1)]];
    float2 texCoord  [[attribute(2)]];
    float3 color     [[attribute(3)]];
};

struct Uniforms {
    float4x4 viewProjection;
    float4x4 model;
};

vertex v2f vertexMain(VertexInput in             [[stage_in]],
                      constant Uniforms& uniforms [[buffer(1)]])
{
    v2f o;
    float4 worldPos = uniforms.model * float4(in.position, 1.0);
    o.position      = uniforms.viewProjection * worldPos;
    o.normal        = normalize((uniforms.model * float4(in.normal, 0.0)).xyz);
    o.texCoord      = in.texCoord;
    o.color         = in.color;
    return o;
}

fragment float4 fragmentMain(v2f in                [[stage_in]])
{
    // ✅ Hardcoded colors
    float3 objectColor = float3(0.0, 0.0, 1.0); // blue
    float3 lightColor  = float3(1.0, 1.0, 1.0); // white

    // ✅ Hardcoded light position
    float3 lightPos = float3(2.0, 100.0, 2.0);

    // --- Ambient ---
    float ambientStrength = 0.2;
    float3 ambient = ambientStrength * lightColor;

    // --- Diffuse ---
    float3 norm = normalize(in.normal);
    float3 lightDir = normalize(lightPos);

    float diff = max(dot(norm, lightDir), 0.0);
    float3 diffuse = diff * lightColor;

    // --- Final color ---
    float3 result = (ambient + diffuse) * objectColor;

    return float4(result, 1.0);
}