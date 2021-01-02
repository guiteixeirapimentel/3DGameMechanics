#pragma once
#include "TexturedQuad.h"

class Fonte
{
public:
	enum ALINHAMENTO
	{
		DIREITA,
		ESQUERDA,
		CENTRO,
		DEFAULT
	};
public:
	Fonte(std::string nomeTextFonte, UINT larguraTexLetra, UINT alturaTexLetra, DirectXC* pDirectX);
	~Fonte();

	void EscreverLetra(char letra, const DirectX::XMFLOAT2& pos,
		const DirectX::XMFLOAT2& tamanhoLetra, Fonte::ALINHAMENTO alinhamento, DirectXC* pDirectX);

	void EscreverFrase(const std::string& frase, const DirectX::XMFLOAT2& pos,
		const DirectX::XMFLOAT2& tamanhoLetra, Fonte::ALINHAMENTO alinhamento, DirectXC* pDirectX);

private:
	TexturedQuad **cLPLetras;

	const UINT cLarguraTexLetra;
	const UINT cAlturaTexLetra;

	UINT cNTotalLetras;
};