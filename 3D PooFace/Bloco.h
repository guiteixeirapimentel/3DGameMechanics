#pragma once
#include "Renderizavel.h"
#include "Cubo.h"

class Bloco : public Renderizavel
{
public:
	Bloco(const std::wstring& arqTextura, DirectXC& dxd);
	~Bloco();

	void SetPosition(const DirectX::XMFLOAT4& pos);

	void Renderizar(DirectXC* pDirectX) const override;

protected:
	Cubo cCubo;

	DirectX::XMFLOAT4 cPos;

};