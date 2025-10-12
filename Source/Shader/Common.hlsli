
#define SIMPLE_MAX_JOINTS 64
#define SIMPLE_MAX_POINTLIGHTS 100

SamplerState GlobalDefaultSampler : register(s0);

Texture2D GlobalAlbedo : register(t0);
Texture2D GlobalNormalTexture : register(t1);
Texture2D GlobalMaterialTexture : register(t2);
Texture2D GlobalAmbientOcclusionAndCustom : register(t3);
TextureCube GlobalCubeMap : register(t4);

Texture2D GlobalBufferAlbedoTexture : register(t5);
Texture2D GlobalBufferNormalTexture : register(t6);
Texture2D GlobalBufferMaterialTexture : register(t7);
Texture2D GlobalBufferPositionTexture : register(t8);
Texture2D GlobalBufferAmbientOcclusionAndCustom : register(t9);

cbuffer CameraBuffer : register(b0)
{
    row_major float4x4 CameraViewMatrix;
    row_major float4x4 CameraProjectionMatrix;
    float3 CameraPosition;
    float paddingCameraBuffer;
    
    uint2 CameraResolution;
    float2 CameraResolutionPadding;
}

cbuffer TransformBuffer : register(b1)
{
    row_major float4x4 ModelToWorldMatrix;
}

cbuffer TimeBuffer : register(b2)
{
    float TotalTime;
    float DeltaTime;
    float2 PaddingTotalTime;
}

cbuffer LightBuffer : register(b3)
{
    float3 AmbientLightColor;
    float1 AmbientLightIntensity;
    float3 DirectionalLightColor;
    float1 DirectionalLightIntensity;

    float3 DirectionLightDirection;
    float1 SpecularIntensity;
    float1 SpecularPower;
};

cbuffer JointBuffer : register(b4)
{
    float4x4 bones[SIMPLE_MAX_JOINTS];
}

cbuffer PostProcessingBuffer : register(b5)
{
    float3 PostProcessTint;
    float PostProcessSaturation;
    
    float PostProcessExposure;
    float PostProcessContrast;
    float PostProcessBlackpoint;
    float PostProcessBloom;
    
    float PostProcessBloomPixelFilterThreshold;
    uint PostProcessUseToneMapping;
    uint PostProcessUseBloom;
    int paddingPostProcess;
}

cbuffer PointLightBufferData : register(b6)
{
    struct PointLightData
    {
        float3 color;
        float intensity;
        float3 position;
        float range;
    } PointLights[SIMPLE_MAX_POINTLIGHTS];
    
    uint UsedPointLights;
    uint3 paddingPointLight;
}

cbuffer SkyBoxBufferData : register(b7)
{
    int SkyboxUseFlooring;
    uint3 skybuxUseFloringPadding;
}

cbuffer ObjectIDBufferData : register(b8)
{
    uint ObjectID;
};

cbuffer ColorBufferData : register(b9)
{
    float4 Color;
};

struct FullScreenVertexInput
{
    unsigned int index : SV_VertexID;
};

struct FullScreenVertexToPixel
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

struct PixelOutput
{
    float4 color : SV_Target;
};

struct GBufferOutput
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 material : SV_TARGET2;
    float4 position : SV_TARGET3;
    float4 ambientOcclusionAndCustom : SV_TARGET4;
};

struct VertexInputType
{
    float4 position : POSITION0;
    float4 color : COLOR0;
    float4 bones : BONES0;
    float4 weights : WEIGHTS0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 bitangent : BITANGENT0;
    float2 uv : TEXCOORD0;
    //float4x4 instanceTransform : INSTANCE_TRANSFORM;
};

struct PixelInputType
{
    float4 position : SV_POSITION0;
    float4 worldPosition : WORLDPOSITION0;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 bitangent : BITANGENT0;
    float2 uv : TEXCOORD0;
};