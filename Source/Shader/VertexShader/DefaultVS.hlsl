#include "../Common.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    float4 vertexWorldPos = mul(input.Position, ModelToWorldMatrix);
    float4 vertexViewPos = mul(vertexWorldPos, CameraViewMatrix);
    
    output.WorldPosition = vertexWorldPos;
    output.Position = mul(vertexViewPos, CameraProjectionMatrix);
    
    output.UV = float2(input.UV.x, 1.0 - input.UV.y);
     
    output.Normal = normalize(mul(input.Normal, (float3x3)ModelToWorldMatrix));
    output.Tangent = normalize(mul(input.Tangent, (float3x3)ModelToWorldMatrix));
    output.Bitangent = normalize(mul(input.Bitangent, (float3x3)ModelToWorldMatrix));
    
    output.Color = input.Color;
    output.ObjectID = ObjectID;
    
    return output;
}

