#include "../Common.hlsli"

float4 SkinVertex(VertexInputType input)
{
    float4 skinnedPosition = float4(0, 0, 0, 0);

    [unroll]
    for (uint i = 0; i < 4; i++)
    {
        uint index = input.Bones[i];
        float weight = input.Weights[i];

        if (weight > 0.0f)
        {
            skinnedPosition += mul(BoneMatrices[index]
            , input.Position) * weight;
        }
    }

    return skinnedPosition;
}

float4 SkinVertex2(VertexInputType input)
{
    float4 vertexObjectPos = input.Position;
    float4x4 skinnedMatrix = 0;
    uint iBone = 0;
    float fWeight = 0;

	// Bone 0
    iBone = input.Bones.x;
    fWeight = input.Weights.x;
    skinnedMatrix += fWeight * BoneMatrices[iBone];

	// Bone 1
    iBone = input.Bones.y;
    fWeight = input.Weights.y;
    skinnedMatrix += fWeight * BoneMatrices[iBone];

	// Bone 2
    iBone = input.Bones.z;
    fWeight = input.Weights.z;
    skinnedMatrix += fWeight * BoneMatrices[iBone];

	// Bone 35
    iBone = input.Bones.w;
    fWeight = input.Weights.w;
    skinnedMatrix += fWeight * BoneMatrices[iBone];

    return mul(transpose(skinnedMatrix), vertexObjectPos);
}

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    float4 skinnedPos = SkinVertex(input);
    float4 vertexWorldPos = mul(ModelToWorldMatrix, skinnedPos);
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


