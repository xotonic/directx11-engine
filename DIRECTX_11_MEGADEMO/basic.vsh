cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 color : COLOR;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	//output.Pos = mul(input.pos, World);
	output.Pos = mul(input.pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.color = input.color;

	return output;
}
