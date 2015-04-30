cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

cbuffer LightBuffer: register(b1)
{
	float4 dir;
	float4 col;
};

Texture2D ObjTexture : register(t0);
Texture2D ObjNormal : register(t1);

SamplerState ObjSamplerState;

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 UV : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 UV : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Norm = mul(input.Norm, World);

	output.UV = input.UV;

	return output;
}

//---------------------------------------

float4 PS(PS_INPUT input) : SV_TARGET
{
	float4 ldir = dir;//float4(-0.577f, 0.0f, -0.5f, 1.0f);
	float4 lcol = col;//float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 color = 0;
	
	float3 tex_norm = (float3)ObjNormal.Sample(ObjSamplerState, input.UV);
	input.Norm = normalize(input.Norm*tex_norm);

	color += saturate(dot((float3)ldir, input.Norm) * lcol* ObjTexture.Sample(ObjSamplerState, input.UV));

	return  color;
}