#include "../Common.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    float4 vertexWorldPos = mul(ModelToWorldMatrix, input.Position);
    float4 vertexViewPos = mul(CameraViewMatrix, vertexWorldPos);
    
    output.WorldPosition = vertexWorldPos;
    output.Position = mul(vertexViewPos, CameraProjectionMatrix);
    
    output.UV = float2(input.UV.x, 1.0 - input.UV.y);
    
    output.Normal = input.Normal;
    output.Tangent = input.Tangent;
    output.Bitangent = input.Bitangent;
   
    output.Color = input.Color;
    output.ObjectID = ObjectID;
    
    return output;
}