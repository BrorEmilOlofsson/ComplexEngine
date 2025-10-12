#include "../Common.hlsli"

float4 EncodeID(uint id)
{
    float r = ((id >> 0) & 0xFF) / 255.0;
    float g = ((id >> 8) & 0xFF) / 255.0;
    float b = ((id >> 16) & 0xFF) / 255.0;
    float a = ((id >> 24) & 0xFF) / 255.0;
    return float4(r, g, b, a);
}

float4 main() : SV_Target
{
    return EncodeID(ObjectID);
}