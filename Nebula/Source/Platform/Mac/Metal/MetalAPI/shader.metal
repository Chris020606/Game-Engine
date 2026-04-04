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
    o.normal        = in.normal;
    o.texCoord      = in.texCoord;
    o.color         = in.color;
    return o;
}

fragment float4 fragmentMain(v2f in                [[stage_in]],
                              texture2d<float> tex [[texture(0)]],
                              sampler texSampler   [[sampler(0)]])
{
    float4 texColor = tex.sample(texSampler, in.texCoord);
    return float4(texColor.rgb, 1.0);
}