#pragma once
#include "Renderizavel.h"
#include "Bloco.h"

#include <vector>

class MapaBlocos : public Renderizavel
{
public:
	MapaBlocos(float comprimento, float largura, DirectXC* pDirectX);
	~MapaBlocos();

	void Renderizar(DirectXC* pDirectX) const override;
private:
	std::vector<Cubo> cMapa;
};