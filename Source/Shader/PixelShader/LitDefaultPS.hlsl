#include "../Common.hlsli"

float1 Square(float1 v)
{
    return v * v;
}

float3 GetUnitVector(float3 start, float3 end)
{
    return normalize(end - start);
}

bool IsInRange(float3 position, float3 position2, float range)
{
    return distance(position, position2) <= range;
}

float3 CalculateSpecularHighlight(float specularIntensity, float specularPower, float3 directionalLightDir, float3 pos, float3 normal, float3 camPos)
{
    const float3 reflectedDirLight = reflect(directionalLightDir, normal);
    const float3 toCam = GetUnitVector(pos, camPos);
    const float1 d = dot(reflectedDirLight, toCam);
    const float1 p = pow(max(0, d), specularPower);
    const float3 specularHighlight = specularIntensity * p;
    
    return specularHighlight;
}

float1 CalculateAttenuation(float1 attConst, float1 attLin, float1 attQuad, float3 worldPos, float3 lightPos)
{
    const float3 vToL = lightPos - worldPos;
    const float1 distanceToLight = length(vToL);
    //const float3 dirTo
    const float att = 1.f / (attConst + attLin * distanceToLight + attQuad * Square(distanceToLight));
    
    return att;
}

float3 CalculateDiffuseNew(float3 worldPos, float3 normal, float3 lightDir, float3 lightColor)
{
    lightDir = normalize(lightDir); // normalize direction

    float NdotL = max(dot(normal, -lightDir), 0.0); // diffuse contribution
    return lightColor * NdotL;
}

float CalculatePointLightAttenuation(float distance, float range)
{
    return 1.0 / (1.0 + distance * distance);
    //return saturate(1.0 - distance / range); // simple linear falloff
}

float3 CalculatePointLightColor(float3 worldPos, float3 normal, float3 pointLightPos, float3 pointLightColor, float pointLightRange, float1 pointLightIntensity)
{
    float3 diffuse = CalculateDiffuseNew(worldPos, normal, normalize(worldPos - pointLightPos), pointLightColor);
    float attenuation = CalculatePointLightAttenuation(distance(worldPos, pointLightPos), pointLightRange);
    return diffuse * attenuation * pointLightIntensity;
}

static const float1 AttConst = 1.f;
static const float1 AttLin = 0.045f;
static const float1 AttQuad = 0.0075f;

float3 CalculateDirectionalLightColor(float3 worldPos, float3 pixelNormal, float3 lightDir, float3 lightColor, float1 lightIntensity)
{
    float4 attenuation = CalculateAttenuation(AttConst, AttLin, AttQuad, worldPos, worldPos - lightDir);
    const float1 d = dot(-lightDir, pixelNormal);
    const float3 diffuse = attenuation.rgb * d * lightColor * lightIntensity;
    return saturate(diffuse);
}

float3 CalculatePointLight(PixelInputType input)
{
    float3 color = float3(0, 0, 0);
    for (uint i = 0; i < UsedPointLights; i++)
    {
        if (IsInRange(input.worldPosition.xyz, PointLights[i].position, PointLights[i].range))
        {
            color += CalculatePointLightColor(
                input.worldPosition.xyz,
                input.normal,
                PointLights[i].position,
                PointLights[i].color.rgb,
                PointLights[i].range,
                PointLights[i].intensity
            );
        }
    }
    
    return saturate(color);
    
}

float4 CalculatePixelColor(float3 directionalLightDir, float3 directionalLightColor, float1 directionalLightIntensity,
    float3 ambientLightColor, float1 ambientLightIntensity, float1 specularIntensity, float1 specularPower,
    float3 camPos, float3 albedoColor, PixelInputType input)
{
    
    const float3 directionalLight = CalculateDirectionalLightColor(input.worldPosition.xyz, input.normal, directionalLightDir, directionalLightColor, directionalLightIntensity);

    const float3 specularColor = CalculateSpecularHighlight(specularIntensity, specularPower, directionalLightDir, input.worldPosition.xyz, input.normal, camPos);
    
    const float3 ambientLight = ambientLightColor * ambientLightIntensity;
    const float3 pointLightColor = CalculatePointLight(input);
    const float3 diffuse = albedoColor * (ambientLight + directionalLight + pointLightColor);

    const float3 finalColor = diffuse + specularColor;
    
    return float4(saturate(finalColor), 1.0);
}

float4 main(PixelInputType input) : SV_TARGET
{
    float3 albedo = GlobalAlbedo.Sample(GlobalDefaultSampler, input.uv).rgb;

    return CalculatePixelColor(
        DirectionLightDirection,
        DirectionalLightColor,
        DirectionalLightIntensity,
        AmbientLightColor,
        AmbientLightIntensity,
        SpecularIntensity,
        SpecularPower,
        CameraPosition,
        albedo,
        input
    );

}