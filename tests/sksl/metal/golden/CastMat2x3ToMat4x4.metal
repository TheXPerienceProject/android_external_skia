#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
struct Inputs {
};
struct Outputs {
    float4 sk_FragColor [[color(0)]];
};
float4x4 float4x4_from_float2x3(float2x3 x0) {
    return float4x4(float4(x0[0].xyz, 0.0), float4(x0[1].xyz, 0.0), float4(0.0, 0.0, 1.0, 0.0), float4(0.0, 0.0, 0.0, 1.0));
}
fragment Outputs fragmentMain(Inputs _in [[stage_in]], bool _frontFacing [[front_facing]], float4 _fragCoord [[position]]) {
    Outputs _outputStruct;
    thread Outputs* _out = &_outputStruct;
    _out->sk_FragColor.x = float(all(float4x4(6.0)[1] == float4x4_from_float2x3(float2x3(7.0))[1]) ? 0 : 1);
    return *_out;
}
