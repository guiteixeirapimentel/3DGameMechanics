#include "PiperPA18.h"

PiperPA18::PiperPA18(DirectXC& dxd, DirectX::XMFLOAT4 cPos)
	:
	cPModelo(nullptr),
	cPos({ cPos.x, cPos.y, cPos.z }),
	cVel({0.0f, 0.0f, 0.0f}),
	cAcel({ 0.0f, 0.0f, 0.0f }),
	cRotY(0.0f),
	cRotX(0.0f),
	cRotZ(0.0f)
{
	cPModelo = new Modelo(
		dxd,
		"Data/pipePA18/piper_pa18_triangulated_CG_origin.obj",
		L"Data/pipePA18/textures/piper_diffuse.jpg"
	);

}

PiperPA18::~PiperPA18()
{
	if (cPModelo)
	{
		delete cPModelo;
		cPModelo = nullptr;
	}
}

void PiperPA18::Renderizar(DirectXC* pDirectX) const
{
	DirectX::XMFLOAT4X4 worldMatrix;
	
	DirectX::XMFLOAT4 poss = { cPos.m128_f32[0], cPos.m128_f32[1], cPos.m128_f32[2], 1.0f };

	DirectX::XMMATRIX mTransRot = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&poss));
	mTransRot = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationY(cRotY), mTransRot);
	mTransRot = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationX(cRotX), mTransRot);
	mTransRot = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationZ(cRotZ), mTransRot);
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

void PiperPA18::Atualizar(float dt)
{
	cAcel = { 0.0f, 0.0f, 0.0f };

	cAcel = DirectX::XMVectorAdd(cAcel, cAcelGravidade);

	cAcel = DirectX::XMVectorScale(cAcel, dt);

	cVel = DirectX::XMVectorAdd(cVel, cAcel);

	DirectX::XMVECTOR velDt = DirectX::XMVectorScale(cVel, dt);

	cPos = DirectX::XMVectorAdd(cPos, velDt);

	if (cPos.m128_f32[1] < 0.0f) {
		cVel.m128_f32[1] = 0.0f;
		cPos.m128_f32[1] = 50.0f;
	}
}
