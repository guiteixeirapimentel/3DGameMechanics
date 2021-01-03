#pragma once
#include "Modelo.h"

class Chao : public Modelo
{
public:
	Chao(float largura, float comprimento, const std::wstring& nomeTextura, DirectXC& dxd);
	Chao(float largura, float comprimento, const std::string& nomeTextura, DirectXC& dxd);
	~Chao();
};