#include "../Common.hlsli"

float4 main(TestVSOut input) : SV_TARGET
{
    float2 uv = input.UV;

    float3 albedo = GBufferAlbedoTexture.Sample(GlobalDefaultSampler, uv).rgb;
    float3 normal = normalize(GBufferNormalTexture.Sample(GlobalDefaultSampler, uv).xyz * 2 - 1);
    float3 position = GBufferPositionTexture.Sample(GlobalDefaultSampler, uv).xyz;

    float3 lightDir = DirectionalLightDirection;
    float intensity = saturate(dot(normal, -lightDir));
    
    float3 finalColor = albedo * intensity;

    return float4(finalColor, 1.0);
    
    //return float4(1, 0, 1, 1); // Debug magenta // works
}