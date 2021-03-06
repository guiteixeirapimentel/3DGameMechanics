#pragma once
#include "Textura.h"
#include <string>

#include <vector>

class Modelo
{
public:
	Modelo(DirectXC& dxd, std::string nomeArqModelo, std::wstring nomeArqTextura);
	virtual ~Modelo();

	virtual void Renderizar(DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 worldInvTranspose, DirectX::XMFLOAT4X4 worldViewProj,
		DirectXC& dxd) const;

	void Rerenderizar(DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 worldInvTranspose, DirectX::XMFLOAT4X4 worldViewProj,
		DirectXC& dxd) const;
protected:
	Modelo();

protected:
	ID3D11Buffer *cPVertexBuffer;
	ID3D11Buffer *cPIndexBuffer;

	const UINT cStride;
	const UINT cOffset;

	size_t cIndexNum;

	Textura *cPTextura;

	Material cMaterial;
};