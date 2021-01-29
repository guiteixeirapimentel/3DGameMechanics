#pragma once
#include "Cubo.h"
#include "Renderizavel.h"

class VasoPlantaBananeira : public Renderizavel
{
public:
	VasoPlantaBananeira(DirectXC& dxd, DirectX::XMFLOAT4 cPos);
	~VasoPlantaBananeira();

	void Renderizar(DirectXC* pDirectX) const override;

	void Atualizar();

private:
	const Modelo* cPModelo;
	DirectX::XMFLOAT4 cPos;

	float cRotY;

};