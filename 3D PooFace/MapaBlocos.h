#pragma once
#include "Renderizavel.h"
#include "Bloco.h"

#include "BlocoAr.h"
#include "BlocoGrama.h"
#include "BlocoPedra.h"

#include <vector>
#include <stdint.h>

class MapaBlocos : public Renderizavel
{
public:
	MapaBlocos(uint32_t largura, uint32_t altura, uint32_t comprimento, DirectXC* pDirectX);
	~MapaBlocos();

	void Renderizar(DirectXC* pDirectX) const override;
private:
	const uint32_t cLargura;
	const uint32_t cAltura;
	const uint32_t cComprimento;

	std::vector<Bloco*> cMapa;

	BlocoAr cBlocoAr;
	BlocoPedra cBlocoPedra;
	BlocoGrama cBlocoGrama;

};