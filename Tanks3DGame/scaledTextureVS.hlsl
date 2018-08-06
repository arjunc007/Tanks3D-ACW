struct VS_INPUT
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 norm : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};


struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 worldPos : TEXCOORD0;
	float2 tex : TEXCOORD1;
	float3 normal : TEXCOORD2;
};

cbuffer uniforms : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

PS_INPUT VShader(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.position = mul(float4(input.pos, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.worldPos = mul(float4(input.pos, 1.0f), worldMatrix);
	output.tex = input.tex * 4.5f;
	output.normal = input.norm;

	return output;
}