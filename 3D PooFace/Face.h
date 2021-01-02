#pragma once
#include "Cubo.h"
#include "Renderizavel.h"

class Face : public Renderizavel
{
public:
	Face(DirectX::XMFLOAT3 posInicial, DirectXC* pDirectX);
	~Face();

	void Renderizar(DirectXC* pDxd) const override;
	
	void ApertouCima();
	void ApertouBaixo();
	void ApertouDireita();
	void ApertouEsquerda();

	void RodouBolinhaMouse(float d);
	void MudouPosMouse(DirectX::XMFLOAT2 dPos);

	void PosicionarCamera(class Camera* pCamera);

	void Atualizar();

	void Rodar()
	{
		cRotacao.y += 0.035f;
	}

	DirectX::XMFLOAT3 PegarPosFace() const
	{
		return cPos;
	}
private:
	DirectX::XMFLOAT3 cPos;
	DirectX::XMFLOAT3 cRotacao;

	//Relativo à face
	DirectX::XMFLOAT4 cPosCamera;
	DirectX::XMFLOAT4 cPosOlhando;

	DirectX::XMFLOAT3 cVel;
	const DirectX::XMFLOAT3 cAcel;

	Cubo* cPCubo;

	bool cAcelerando;
};