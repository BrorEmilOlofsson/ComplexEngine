#include "../Common.hlsli"

//float4 EncodeObjectID(uint id)
//{
//    float r = ((id >> 0) & 0xFF) / 255.0;
//    float g = ((id >> 8) & 0xFF) / 255.0;
//    float b = ((id >> 16) & 0xFF) / 255.0;
//    float a = ((id >> 24) & 0xFF) / 255.0;
//    return float4(r, g, b, a);
//}

GBufferPSOutput main(PixelInputType input)
{
    float3 albedo = GlobalAlbedoTexture.Sample(GlobalDefaultSampler, input.UV).rgb;
    float1 specular = 0;
    GBufferPSOutput o;
    o.Albedo = float4(albedo, 1);
    o.Normal = float4(input.Normal * 0.5f + 0.5f, 1);
    o.Material = float4(0, 0, 0, 0);
    o.Position = float4(input.WorldPosition.xyz, 1.0f);
    o.ObjectID = ObjectID;
    return o;
}