#pragma once
#include "Cubo.h"
#include "Renderizavel.h"

class Poop : public Renderizavel
{
public:
	Poop(DirectX::XMFLOAT4 cPos, const Cubo* pCubo);
	~Poop();

	void Renderizar(DirectXC* pDirectX) const override;

	void Atualizar();

	bool TestarColisaoFace(const class Face* pFace) const;
private:
	const Cubo* cPCubo;
	DirectX::XMFLOAT4 cPos;

	float cTempo;
};