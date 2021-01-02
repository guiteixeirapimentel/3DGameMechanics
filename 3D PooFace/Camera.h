#pragma once
#include "Renderizador.h"
#include <DirectXMath.h>

class Camera : public Renderizador
{
public:
	Camera(DirectX::XMFLOAT4 posInicial, DirectX::XMFLOAT4 rotacaoInicial, DirectX::XMFLOAT4 posOlhandoInicial,
		Renderizador& proximo);

	void Atualizar();

	void Renderizar(Renderizavel* pRenderizavel) override;
	
	DirectX::XMFLOAT4 PegarPosOlho() const;
	DirectX::XMFLOAT4 PegarPosOlhando() const;

	void SetarPosOlhando(DirectX::XMFLOAT4 nPos);
	void SetarPosOlho(DirectX::XMFLOAT4 nPos);
private:
	Renderizador& cProximo;

	DirectX::XMFLOAT4 cPos;
	DirectX::XMFLOAT4 cPosOlhando;
	DirectX::XMFLOAT4 cRotacao;

	DirectX::XMFLOAT4X4 cViewMatrix;
};