#include "../VertexShader/FullScreenVS.hlsl"

Texture2D sceneTex : register(t0);
SamplerState sceneSampler : register(s0);

float4 main(FullScreenVSOut input) : SV_Target
{
    return sceneTex.Sample(sceneSampler, input.uv);
}