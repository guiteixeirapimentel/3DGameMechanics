#pragma once
#include "Textura.h"
#include <string>

#include <vector>

class ModeloInstancia
{
public:
	ModeloInstancia(DirectXC& dxd, std::string nomeArqModelo, std::string nomeArqTextura);
	virtual ~ModeloInstancia();

	virtual void Renderizar(DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 worldInvTranspose, DirectX::XMFLOAT4X4 worldViewProj,
		DirectXC& dxd) const;

protected:
	ModeloInstancia();

protected:
	ID3D11Buffer *cPVertexBuffer;
	ID3D11Buffer *cPIndexBuffer;

	const UINT cStride;
	const UINT cOffset;

	size_t cIndexNum;

	Textura *cPTextura;

	Material cMaterial;
};