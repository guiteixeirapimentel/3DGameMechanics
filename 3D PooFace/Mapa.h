#pragma once
#include "Renderizavel.h"
#include "Chao.h"

#include <vector>

class Mapa : public Renderizavel
{
public:
	Mapa(float comprimento, float largura, DirectXC* pDirectX);
	~Mapa();

	void Renderizar(DirectXC* pDirectX) const override;
private:
	Chao cChao;
};