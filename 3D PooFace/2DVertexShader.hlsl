struct VertexInputType
{
	float4 posL : POSICAO;
	float4 textCoord : TEXTCOORD;
};

struct PixelInputType
{
	float4 posH : SV_POSITION;
	float4 textCoord : TEXTCOORD;
};

cbuffer PorObjeto : register(b0)
{
	matrix cBWorldMatrix;
};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;

	output.posH = mul(input.posL, cBWorldMatrix);

	output.textCoord = input.textCoord;

	return output;
}