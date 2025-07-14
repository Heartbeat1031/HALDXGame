struct VS_IN
{
    float4 Position : POSITION0;
    float4 Diffuse  : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

float4 main(in PS_IN In) : SV_Target
{
    float4 outDiffuse;
	
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;
    
    return outDiffuse;
}

