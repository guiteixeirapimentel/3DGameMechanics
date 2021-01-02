struct PixelInputType
{
	float4 posH : SV_POSITION;
	float4 textCoord : TEXTCOORD;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(PixelInputType input) : SV_TARGET
{
	float4 texCor = shaderTexture.Sample(SampleType, input.textCoord);

	return texCor;
}