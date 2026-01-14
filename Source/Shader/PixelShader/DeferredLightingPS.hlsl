#include "../Common.hlsli"


float3 CalculatePixelColor(float3 albedo, float3 pixelNormal, float3 pixelPosition, float3 cameraPosition, float3 directionalLightDirection, float3 ambientLightColor, float ambientLightIntensity)
{
    float3 L = normalize(-directionalLightDirection); // toward light
    float3 V = normalize(cameraPosition - pixelPosition); // toward camera
    
    float NdotL = saturate(dot(pixelNormal, L));

    // Softer, nicer diffuse (wrap lighting)
    float wrap = 0.25;
    float diffuse = saturate((NdotL + wrap) / (1.0 + wrap));
    
    
    // Simple specular (Blinn-Phong)
    float3 H = normalize(L + V);
    float specPow = 64.0;
    float specStrength = 0.04;
    float specular = pow(saturate(dot(pixelNormal, H)), specPow) * specStrength;

    // Subtle fresnel tweak (optional)
    float fresnel = pow(1.0 - saturate(dot(pixelNormal, V)), 5.0);
    specular *= lerp(1.0, 2.0, fresnel);

    float3 ambient = albedo * (ambientLightColor * ambientLightIntensity);

    float3 finalColor = albedo * diffuse + specular + ambient;
    return finalColor;
}

float4 main(TestVSOut input) : SV_TARGET
{
    float2 uv = input.UV;

    float3 albedo = GBufferAlbedoTexture.Sample(GlobalDefaultSampler, uv).rgb;
    float3 normal = normalize(GBufferNormalTexture.Sample(GlobalDefaultSampler, uv).xyz * 2 - 1);
    float3 position = GBufferPositionTexture.Sample(GlobalDefaultSampler, uv).xyz;

    
    float3 finalColor = CalculatePixelColor(
        albedo,
        normal,
        position,
        CameraPosition,
        DirectionalLightDirection,
        AmbientLightColor,
        AmbientLightIntensity
    );
    
    return float4(finalColor, 1.0);
}