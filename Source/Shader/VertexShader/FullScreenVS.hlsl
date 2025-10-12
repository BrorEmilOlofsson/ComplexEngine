struct FullScreenVSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

FullScreenVSOut main(uint id : SV_VertexID)
{
    FullScreenVSOut output;

    // 3 hardcoded vertices covering screen
    float2 verts[3] =
    {
        float2(-1.0f, -1.0f),
        float2(-1.0f, 3.0f),
        float2(3.0f, -1.0f)
    };

    float2 pos = verts[id];
    output.pos = float4(pos, 0.0f, 1.0f);
    output.uv = float2((pos.x + 1.0f) * 0.5f, 1.0f - (pos.y + 1.0f) * 0.5f);
    return output;
}