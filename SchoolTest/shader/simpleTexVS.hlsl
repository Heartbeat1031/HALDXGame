struct VS_IN
{
    float4 Position : POSITION0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

PS_IN main(VS_IN In)
{
    PS_IN Out;
	
    Out.Position = In.Position;
    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse;
	
    return Out;
}