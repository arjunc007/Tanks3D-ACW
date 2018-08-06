Texture2D shaderTexture : register (t0);
SamplerState sampleType : register(s0);

static const float4 LightColor = float4(1, 1, 1, 1);
static const float3 LightPos[2] = { float3(13, 10, 0), float3(0, 10, 11) };

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

cbuffer camera : register(b1)
{
	float4 Eye;
}

PS_INPUT VShader(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.position = mul(float4(input.pos, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.worldPos = mul(float4(input.pos, 1.0f), worldMatrix);
	output.tex = input.tex;
	output.normal = input.norm;

	return output;
}

float4 Phong(float3 n, float3 l, float3 v, float shininess, float4 diffuseColor, float4 specularColor)
{
	float NdotL = dot(n, l);
	float diff = saturate(NdotL);
	float3 r = reflect(l, n);
	float spec = pow(saturate(dot(v, r)), shininess) * (NdotL > 0.0);
	return diff * diffuseColor + spec * specularColor;
}

float4 PShader(PS_INPUT input) : SV_TARGET
{
	float4 diff = shaderTexture.Sample(sampleType, input.tex);
	float4 spec = float4(1, 1, 1, 1);

	float3 viewDir = normalize(Eye.xyz - input.worldPos.xyz);
	float3 lightDir;
	float4 color = (float4)0;

	for (int i = 0; i < 2; i++)
	{
		lightDir = normalize(LightPos[i] - input.worldPos.xyz);
		color += Phong(input.normal, lightDir, viewDir, 40, diff, spec);
	}

	return saturate(color * LightColor);
}