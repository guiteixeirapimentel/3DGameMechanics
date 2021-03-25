#include "MapaBlocos.h"

MapaBlocos::MapaBlocos(uint32_t largura, uint32_t altura, uint32_t comprimento, DirectXC* pDirectX)
	:
	cLargura(largura),
	cAltura(altura),
	cComprimento(comprimento),
	cBlocoAr(*pDirectX),
	cBlocoPedra(*pDirectX),
	cBlocoGrama(*pDirectX)
{
	cMapa.resize(cLargura * cAltura * cComprimento, &cBlocoAr);

	for (size_t i = 0; i < cLargura; i++)
	{
		//const size_t j = 0;
		for (size_t j = 0; j < cAltura; j++)
		for (size_t k = 0; k < cComprimento; k++)
		{
			cMapa[k + (j*cComprimento) + (i*cComprimento*cAltura)] = &cBlocoPedra;
		}
	}
}
MapaBlocos::~MapaBlocos()
{}

void MapaBlocos::Renderizar(DirectXC* pDirectX) const
{
	// i -> x
	for (size_t i = 0; i < cLargura; i++)
	{
		// j -> y
		for (size_t j = 0; j < cAltura; j++)
		{
			// k-> z
			for (size_t k = 0; k < cComprimento; k++)
			{
				const size_t index = k + (j*cComprimento) + (i *cComprimento*cAltura);

				const DirectX::XMFLOAT4 pos = { static_cast<float>(i), static_cast<float>(j), static_cast<float>(k), 1.0f };

				auto pBloco = cMapa[index];

				if (pBloco->DeveRenderizar())
				{
					cMapa[index]->SetPosition(pos);
					cMapa[index]->SetarViewMatrix(cViewMatrix);
					cMapa[index]->SetarProjectionMatrix(cProjMatrix);

					cMapa[index]->Renderizar(pDirectX);
				}				
			}
		}
	}
}