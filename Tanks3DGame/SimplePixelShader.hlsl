Texture2D shaderTexture : register (t0);
SamplerState sampleType : register(s0);

static const float4 LightColor = float4(1, 1, 1, 1);
static const float3 LightPos[2] = { float3(13, 20, 0), float3(0, 30, 11) };

cbuffer camera : register(b1)
{
	float4 Eye;
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 worldPos : TEXCOORD0;
	float2 tex : TEXCOORD1;
	float3 normal : TEXCOORD2;
};

float4 Phong(float3 n, float3 l, float3 v, float shininess, float4 diffuseColor, float4 specularColor)
{
	float NdotL = dot(n, l);
	float diff = saturate(NdotL);
	float3 r = reflect(l, n);
	float spec = pow(saturate(dot(v, r)), shininess) * (NdotL > 0.0);
	return diff * diffuseColor + spec * specularColor;
}

float4 PShader(PS_INPUT Input) : SV_TARGET
{
	float4 spec = float4(1, 1, 1, 1);
	float4 diff = shaderTexture.Sample(sampleType, Input.tex);

	float4 color = (float4)0;
	float3 lightDir;
	float3 viewDir = normalize(Eye.xyz - Input.worldPos.xyz);

	float3 normal = float3(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < 2; i++)
	{
		lightDir = normalize(LightPos[i].xyz - Input.worldPos.xyz);
		color += Phong(normal, lightDir, viewDir, 50, diff, spec);
	}

	return saturate(LightColor * 0.5f * color);
}