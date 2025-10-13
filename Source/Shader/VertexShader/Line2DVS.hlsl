#include "../Common.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    float4 vertexWorldPos = mul(input.Position, ModelToWorldMatrix);
    float4 vertexViewPos = mul(vertexWorldPos, CameraViewMatrix);
    
    output.WorldPosition = vertexWorldPos;
    output.Position = mul(vertexViewPos, CameraProjectionMatrix);
    
    output.UV = float2(input.UV.x, 1.0 - input.UV.y);
    
    output.Normal = input.Normal;
    output.Tangent = input.Tangent;
    output.Bitangent = input.Bitangent;
   
    output.Color = input.Color;
    
    return output;
}