#include "Bloco.h"


Bloco::Bloco(const std::wstring& arqTextura, DirectXC& dxd)
	:
	cCubo(arqTextura, 1.0f, 1.0f, 1.0f, dxd)
{

}
Bloco::~Bloco()
{

}

void Bloco::SetPosition(const DirectX::XMFLOAT4& pos)
{
	cPos = pos;
}

void Bloco::Renderizar(DirectXC* pDirectX) const
{
	DirectX::XMFLOAT4X4 worldMatrix;
	
	DirectX::XMFLOAT4 poss = { cPos.x, cPos.y, cPos.z, 1.0f };

	DirectX::XMMATRIX mTransRot = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&poss));
	mTransRot = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationX(0.0f), mTransRot);
	DirectX::XMStoreFloat4x4(&worldMatrix, mTransRot);

	DirectX::XMFLOAT4X4 worldInvTranspose;
	DirectX::XMMATRIX A = DirectX::XMLoadFloat4x4(&worldMatrix);
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	DirectX::XMStoreFloat4x4(&worldInvTranspose, DirectX::XMMatrixTranspose(XMMatrixInverse(&det, A)));

	DirectX::XMMATRIX worldViewM = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&worldMatrix), DirectX::XMLoadFloat4x4(&cViewMatrix));
	DirectX::XMMATRIX worldViewProjM = DirectX::XMMatrixMultiply(worldViewM, DirectX::XMLoadFloat4x4(&cProjMatrix));

	DirectX::XMFLOAT4X4 worldViewProj;
	DirectX::XMStoreFloat4x4(&worldViewProj, worldViewProjM);

	cCubo.Renderizar(worldMatrix, worldInvTranspose, worldViewProj, *pDirectX);
}