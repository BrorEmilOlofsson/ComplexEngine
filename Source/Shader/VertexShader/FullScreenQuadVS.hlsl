#include "../Common.hlsli"

TestVSOut main(uint id : SV_VertexID)
{
    TestVSOut o;
    o.UV = float2((id << 1) & 2, id & 2);
    o.Position = float4(o.UV * float2(2, -2) + float2(-1, 1), 0, 1);
    return o;
}