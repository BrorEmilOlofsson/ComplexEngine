#include "../Common.hlsli"

float4 main(VertexInputType input) : SV_POSITION
{
    const float4 worldPosition = mul(input.Position, world);
    return mul(worldPosition, lightViewProjection);
}
