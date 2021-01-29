#include "VasoPlantaBananeira.h"
#include "Face.h"

VasoPlantaBananeira::VasoPlantaBananeira(DirectXC& dxd, DirectX::XMFLOAT4 cPos)
	:
	cPModelo(nullptr),
	cPos(cPos),
	cRotY(0.0f)
{
	cPModelo = new Modelo(
		dxd,
		"Data/panzerConverted.obj",
		L"Data/panzer.jpg"
	);

}

VasoPlantaBananeira::~VasoPlantaBananeira()
{
	if (cPModelo) 
	{
		delete cPModelo;
		cPModelo = nullptr;
	}
}

void VasoPlantaBananeira::Renderizar(DirectXC* pDirectX) const
{
	DirectX::XMFLOAT4X4 worldMatrix;

	DirectX::XMFLOAT4 poss = { cPos.x, cPos.y, cPos.z, 1.0f };

	DirectX::XMMATRIX mTransRot = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&poss));
	mTransRot = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationY(cRotY), mTransRot);
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

	cPModelo->Renderizar(worldMatrix, worldInvTranspose, worldViewProj, *pDirectX);
}

void VasoPlantaBananeira::Atualizar()
{
	if (cRotY < DirectX::XM_PI*2.0f)
	{
		cRotY -= 2.0f*DirectX::XM_PI;
		cRotY += DirectX::XM_PI / 120.0f;
	}
	
}
