#pragma once
#include <DirectXMath.h>

class Renderizavel
{
public:
	Renderizavel();
	~Renderizavel();

	virtual void Renderizar(class DirectXC* dxd) const = 0;

	void SetarViewMatrix(DirectX::XMFLOAT4X4 viewMatrix);
	void SetarProjectionMatrix(DirectX::XMFLOAT4X4 projMatrix);

protected:
	DirectX::XMFLOAT4X4 cProjMatrix;
	DirectX::XMFLOAT4X4 cViewMatrix;
};