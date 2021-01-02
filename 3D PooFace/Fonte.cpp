#include "Fonte.h"

Fonte::Fonte(std::string nomeTextFonte, UINT larguraTexLetra, UINT alturaTexLetra, DirectXC* pDirectX)
	:
	cLarguraTexLetra(larguraTexLetra),
	cAlturaTexLetra(alturaTexLetra),
	cLPLetras(NULL)
{
	char* pPixelDataTexture = NULL;
	UINT alturaTextura = 0;
	UINT larguraTextura = 0;

	std::ifstream arqTex(nomeTextFonte, std::ios::binary);

	if (arqTex.is_open())
	{
		char c = 0;
		arqTex >> c;
		arqTex >> c;
		arqTex >> c;
		arqTex >> c;

		arqTex >> alturaTextura;
		arqTex >> c;
		arqTex >> larguraTextura;
		arqTex >> c;

		unsigned int nBytes = alturaTextura * (std::streamsize)larguraTextura * 4;

		pPixelDataTexture = new char[nBytes];

		arqTex.read(pPixelDataTexture, nBytes);

		arqTex.close();
	}
	else
	{
		MessageBox(NULL, L"Erro ao carregar fontes.", L"Faltando arquivo.", MB_ICONERROR);
		return;
	}

	const UINT nLetrasHorizontal = larguraTextura / (UINT)cLarguraTexLetra;
	const UINT nLetrasVertical = alturaTextura / (UINT)cAlturaTexLetra;
	const UINT nTotalLetras = nLetrasHorizontal * nLetrasVertical;

	cNTotalLetras = nTotalLetras;

	cLPLetras = new TexturedQuad*[nTotalLetras];

	//ZeroMemory(pPixelDataTexture, alturaTextura * larguraTextura * 4);

	char* pPixelDataLetra = new char[UINT(cLarguraTexLetra * cAlturaTexLetra * 4)];

	for (UINT i = 0; i < nTotalLetras; i++)
	{
		const UINT xOffset = cLarguraTexLetra * i;
		const UINT yOffset = cLarguraTexLetra * (cAlturaTexLetra - 1) * nLetrasHorizontal * (i / nLetrasHorizontal);
		for (UINT y = 0; y < cAlturaTexLetra; y++)
		{
			for (UINT x = 0; x < cLarguraTexLetra; x++)
			{
				const UINT index = x + xOffset + (y * larguraTextura) + yOffset;

				reinterpret_cast<int*>(pPixelDataLetra)[x + y * larguraTexLetra] =
					reinterpret_cast<int*>(pPixelDataTexture)[index];

				if (reinterpret_cast<int*>(pPixelDataTexture)[index] != 0x00FFFFFF && reinterpret_cast<int*>(pPixelDataTexture)[index] != 0xFF000000)
				{
					int xz = 0;
				}
			}
		}

		cLPLetras[i] = new TexturedQuad(pPixelDataLetra, cLarguraTexLetra, cAlturaTexLetra,
			1.0f, 1.0f, { 0.0f, 0.0f }, pDirectX);
	}

	delete[] pPixelDataLetra;
	pPixelDataLetra = NULL;

	delete[] pPixelDataTexture;
	pPixelDataTexture = NULL;
}

Fonte::~Fonte()
{
	if (cLPLetras)
	{
		for (UINT i = 0; i < cNTotalLetras; i++)
		{
			delete cLPLetras[i];
			cLPLetras[i] = NULL;
		}

		delete[] cLPLetras;
	}
}

void Fonte::EscreverLetra(char letra, const DirectX::XMFLOAT2& pos,
	const DirectX::XMFLOAT2& tamanhoLetra, Fonte::ALINHAMENTO alinhamento, DirectXC* pDirectX)
{
	assert(letra - 32 >= 0 && letra - 32 < (int)cNTotalLetras);
	const char c = (letra - 32) % cNTotalLetras;

	switch (alinhamento)
	{
	case ESQUERDA:
	{
		cLPLetras[c]->Renderizar(pos, tamanhoLetra, 0.0f, pDirectX);
	}break;
	case DIREITA:
	{
		cLPLetras[c]->Renderizar({ pos.x - cLarguraTexLetra, pos.y }, tamanhoLetra, 0.0f, pDirectX);
	}break;
	case CENTRO:
	{
		cLPLetras[c]->Renderizar({ pos.x - (cLarguraTexLetra / 2.0f), pos.y }, tamanhoLetra, 0.0f, pDirectX);
	}break;
	default:
	{
		cLPLetras[c]->Renderizar(pos, tamanhoLetra, 0.0f, pDirectX);
	}break;
	}
}

void Fonte::EscreverFrase(const std::string& frase, const DirectX::XMFLOAT2& pos,
	const DirectX::XMFLOAT2& tamanhoLetra, Fonte::ALINHAMENTO alinhamento, DirectXC* pDirectX)
{
	DirectX::XMFLOAT2 deltaPos = { 0.0f, 0.0f };
	switch (alinhamento)
	{
	case Fonte::ESQUERDA:
	{
		//Nada acontece
	}break;
	case Fonte::DIREITA:
	{
		deltaPos.x = -float(frase.length()) * tamanhoLetra.x;
	}break;
	case Fonte::CENTRO:
	{
		deltaPos.x = (-float(frase.length()) * tamanhoLetra.x) / 2.0f;
	}break;
	default:
	{
		//Nada 
	}break;
	}

	for (size_t i = 0; i < frase.length(); i++)
	{
		DirectX::XMFLOAT2 posLetra = { pos.x + (i * tamanhoLetra.x) + deltaPos.x, pos.y + deltaPos.y };
		EscreverLetra(frase.at(i), posLetra, tamanhoLetra, Fonte::ESQUERDA, pDirectX);
	}
}