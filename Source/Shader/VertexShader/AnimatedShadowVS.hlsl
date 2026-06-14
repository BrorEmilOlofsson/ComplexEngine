#include "../Common.hlsli"

float4 SkinVertex(VertexInputType input)
{
    float4 skinnedPosition = float4(0, 0, 0, 0);

    [unroll]
    for (uint i = 0; i < 4; i++)
    {
        const uint index = input.Bones[i];
        const float weight = input.Weights[i];

        if (weight > 0.0f)
        {
            skinnedPosition += mul(input.Position, BoneMatrices[index]) * weight;
        }
    }

    return skinnedPosition;
}

float4 main(VertexInputType input) : SV_POSITION
{
    const float4 worldPosition = mul(SkinVertex(input), world);
    return mul(worldPosition, lightViewProjection);
}
