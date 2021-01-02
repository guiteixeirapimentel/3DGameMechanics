#pragma once
#include "Modelo.h"

class Cubo : public Modelo
{
public:
	Cubo(std::string nomeTextura, float altura, float largura, float profundidade, DirectXC& dxd);
	~Cubo();
};