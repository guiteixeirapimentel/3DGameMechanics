#pragma once
#include "DirectXC.h"
#include <string>
#include <fstream>

class Textura
{
public:
	Textura(DirectXC& dxd, std::string nomeArqTextura);
	~Textura();

	ID3D11ShaderResourceView *PegarTextura() const;

	UINT PegarLargura() const;
	UINT PegarAltura() const;

private:
	void PegarDataImg(std::string nomeArq, char **data, UINT *largura, UINT *altura);
private:
	ID3D11ShaderResourceView* cPTextura;

	UINT cLargura;
	UINT cAltura;
};