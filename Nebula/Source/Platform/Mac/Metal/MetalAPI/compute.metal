#include <metal_stdlib>
using namespace metal;

kernel void kernelMain(texture2d<half, access::write> tex [[texture(0)]],
                       uint2 index                        [[thread_position_in_grid]],
                       uint2 gridSize                     [[threads_per_grid]],
                       device const float* time           [[buffer(0)]])
{
    constexpr float kAnimationFrequency = 0.01;
    constexpr float kAnimationSpeed     = 4;
    constexpr float kAnimationScaleLow  = 0.62;
    constexpr float kAnimationScale     = 0.38;

    constexpr float2 kMandelbrotPixelOffset = {-0.2, -0.35};
    constexpr float2 kMandelbrotOrigin      = {-1.2, -0.32};
    constexpr float2 kMandelbrotScale       = {2.2,  2.0};

    float zoom = kAnimationScaleLow + kAnimationScale * cos(kAnimationFrequency * *time);
    zoom = pow(zoom, kAnimationSpeed);

    float x0 = zoom * kMandelbrotScale.x * ((float)index.x / gridSize.x + kMandelbrotPixelOffset.x) + kMandelbrotOrigin.x;
    float y0 = zoom * kMandelbrotScale.y * ((float)index.y / gridSize.y + kMandelbrotPixelOffset.y) + kMandelbrotOrigin.y;

    float x = 0.0;
    float y = 0.0;
    uint  iteration     = 0;
    uint  max_iteration = 10000;
    float xtmp = 0.0;

    while (x * x + y * y <= 4 && iteration < max_iteration) {
        xtmp = x * x - y * y + x0;
        y    = 2 * x * y + y0;
        x    = xtmp;
        iteration += 1;
    }

    half color = (0.5 + 0.5 * cos(3.0 + iteration * 0.15));
    tex.write(half4(color + 0.2, color, color + 1, 1.0), index, 0);
}