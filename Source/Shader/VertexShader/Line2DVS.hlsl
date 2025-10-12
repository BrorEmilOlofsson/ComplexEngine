#include "../Common.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    float4 vertexWorldPos = mul(input.position, ModelToWorldMatrix);
    float4 vertexViewPos = mul(vertexWorldPos, CameraViewMatrix);
    
    output.worldPosition = vertexWorldPos;
    output.position = mul(vertexViewPos, CameraProjectionMatrix);
    
    output.uv = float2(input.uv.x, 1.0 - input.uv.y);
    
    output.normal = input.normal;
    output.tangent = input.tangent;
    output.bitangent = input.bitangent;
   
    output.color = input.color;
    
    return output;
}