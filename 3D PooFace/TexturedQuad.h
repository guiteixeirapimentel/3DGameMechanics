#pragma once
#include "DirectXC.h"

class TexturedQuad
{
public:
	TexturedQuad(std::string nomeImg, float largura, float altura, DirectX::XMFLOAT2 centerPos, DirectXC* pDirectX);
	TexturedQuad(std::string nomeImg, DirectX::XMFLOAT2 centerPos, DirectXC* pDirectX);

	TexturedQuad(const void* pPixels, UINT larguraTextura, UINT alturaTextura, float larguraQuad, float alturaQuad,
		DirectX::XMFLOAT2 centerPos, DirectXC* pDirectX);

	~TexturedQuad();

	void Renderizar(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 escala, float rotacao, DirectXC* pDirectX) const;

private:
	void CriarQuadBuffers(DirectX::XMFLOAT2 centerPos, DirectXC* pDirectX);
	void CriarBufferTextura(const void* pPixelData, UINT larguraText, UINT alturaText, DirectXC* pDirectX);

private:
	ID3D11Buffer* cPVertexBuffer;
	ID3D11Buffer* cPIndexBuffer;

	ID3D11ShaderResourceView* cPTextura;

	const UINT cStride;
	const UINT cOffset;

	const UINT cIndexNum;

	float cAltura;
	float cLargura;
};