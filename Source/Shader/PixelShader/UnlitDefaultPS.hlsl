#include "../Common.hlsli"

float4 main(PixelInputType input) : SV_TARGET
{   
    return Color;
    //return float4(1, 0, 0, 1);
    //return float4(input.color.rgb, 1);
}