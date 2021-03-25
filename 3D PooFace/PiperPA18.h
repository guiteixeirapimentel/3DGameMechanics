#pragma once
#include "Modelo.h"
#include "Renderizavel.h"

class PiperPA18 : public Renderizavel
{
public:
	PiperPA18(DirectXC& dxd, DirectX::XMFLOAT4 cPos);
	~PiperPA18();

	void Renderizar(DirectXC* pDirectX) const override;

	void Atualizar(float dt = 1.0f/60.0f);

private:
	const Modelo* cPModelo;
	DirectX::XMVECTOR cPos;

	const float cMassa = 608.0f; // KG
	const float cEmpuxo = 1000.0f; //N
	const float cDistCGOrigem = 0.0f; // m

	const DirectX::XMVECTOR cAcelGravidade = {0.0f, -9.81f, 0.0f};
	float cRotY, cRotX, cRotZ;

	DirectX::XMVECTOR cVel;
	DirectX::XMVECTOR cAcel;

};