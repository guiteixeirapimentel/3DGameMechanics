#pragma once
#include "Modelo.h"

class Chao : public Modelo
{
public:
	Chao(float largura, float comprimento, std::string nomeTextura, DirectXC& dxd);
	~Chao();
};