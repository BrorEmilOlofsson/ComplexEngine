#include "../Common.hlsli"

GBufferPSOutput main(PixelInputType input)
{
    float3 albedo = GlobalAlbedo.Sample(GlobalDefaultSampler, input.UV).rgb;
    float1 specular = 0;
    GBufferPSOutput o;
    o.Albedo = float4(albedo, 0);
    o.Normal = float4(normalize(input.Normal) * 0.5f + 0.5f, specular);
    o.Position = float4(input.WorldPosition.xyz, 1.0f);
    o.Material = float4(0, 0, 0, 0);
    return o;
}