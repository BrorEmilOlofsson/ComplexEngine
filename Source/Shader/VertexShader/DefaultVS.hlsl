#include "../Common.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    float4 vertexWorldPos = mul(ModelToWorldMatrix, input.Position);
    float4 vertexViewPos = mul(CameraViewMatrix, vertexWorldPos);
    
    output.WorldPosition = vertexWorldPos;
    output.Position = mul(CameraProjectionMatrix, vertexViewPos);
    
    output.UV = float2(input.UV.x, 1.0 - input.UV.y);
     
    output.Normal = normalize(mul((float3x3) ModelToWorldMatrix, input.Normal));
    output.Tangent = normalize(mul((float3x3) ModelToWorldMatrix, input.Tangent));
    output.Bitangent = normalize(mul((float3x3) ModelToWorldMatrix, input.Bitangent));
    
    output.Color = input.Color;
    output.ObjectID = ObjectID;
    
    return output;
}

