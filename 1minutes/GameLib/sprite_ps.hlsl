#include "sprite.hlsli"
Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register(s0);
float4 main(VS_OUT pin) : SV_TARGET
{
    float4 col = diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord) * pin.color;
    if (col.a <= 0.0f) discard;
    return col;
}