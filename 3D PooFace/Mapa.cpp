#include "Mapa.h"

Mapa::Mapa(float comprimento, float largura, DirectXC* pDirectX)
	:
	cChao(largura, comprimento, L"Data\\Textures\\grasstexture.jpg", *pDirectX)
{
}

Mapa::~Mapa()
{}

void Mapa::Renderizar(DirectXC* pDirectX) const
{
	DirectX::XMFLOAT4X4 worldMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());

	DirectX::XMFLOAT4X4 worldInvTranspose;
	DirectX::XMMATRIX A = DirectX::XMLoadFloat4x4(&worldMatrix);
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	DirectX::XMStoreFloat4x4(&worldInvTranspose, DirectX::XMMatrixTranspose(XMMatrixInverse(&det, A)));

	DirectX::XMMATRIX worldViewM = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&worldMatrix), DirectX::XMLoadFloat4x4(&cViewMatrix));
	DirectX::XMMATRIX worldViewProjM = DirectX::XMMatrixMultiply(worldViewM, DirectX::XMLoadFloat4x4(&cProjMatrix));

	DirectX::XMFLOAT4X4 worldViewProj;
	DirectX::XMStoreFloat4x4(&worldViewProj, worldViewProjM);

	cChao.Renderizar(worldMatrix, worldInvTranspose, worldViewProj, *pDirectX);
}