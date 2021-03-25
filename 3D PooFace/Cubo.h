#pragma once
#include "Modelo.h"

class Cubo : public Modelo
{
public:
	Cubo(const std::string& nomeTextura, float altura, float largura, float profundidade, DirectXC& dxd);
	Cubo(const std::wstring& nomeTextura, float altura, float largura, float profundidade, DirectXC& dxd);

	~Cubo();

private:
	void LoadVertexStuff(float altura, float largura, float profundidade, DirectXC& dxd);
};