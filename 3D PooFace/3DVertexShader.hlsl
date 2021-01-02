struct VertexInputType
{
	float3 posL : POSICAO;
	float3 normalL : NORMAL;
	float2 textCoord : TEXTCOORD;
};

struct PixelInputType
{
	float4 posH : SV_POSITION;
	float3 posW : POSICAOWORLD;
	float3 normalW : NORMAL;
	float2 textCoord : TEXTCOORD;
};

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

cbuffer PorObjeto : register(b0)
{
	matrix cBWorldMatrix;
	matrix CBWorldInvTranspose;
	matrix CBWorldViewProj;
	Material cBMaterial;
};


PixelInputType VS(VertexInputType input)
{
	PixelInputType output;

	output.posW = mul(float4(input.posL, 1.0f), cBWorldMatrix).xyz;
	output.normalW = mul(input.normalL, (float3x3)CBWorldInvTranspose);
	output.posH = mul(float4(input.posL, 1.0f), CBWorldViewProj);
	output.textCoord = input.textCoord;

	return output;
}