#include "../Common.hlsli"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    float4 vertexObjectPos = input.position;
    float4 vertexWorldPos = mul(vertexObjectPos, ModelToWorldMatrix);
    
    output.position = vertexWorldPos;
    output.worldPosition = vertexWorldPos;
    output.uv = float2(input.uv.x, 1.0 - input.uv.y);
    
    output.normal = input.normal;
    output.tangent = input.tangent;
    output.bitangent = input.bitangent;
    
    output.color = input.color;
    
    return output;
}
