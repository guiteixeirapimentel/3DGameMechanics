#include "Face.h"
#include "Camera.h"

Face::Face(DirectX::XMFLOAT3 posInicial, DirectXC *pDirectX)
	:
	cPCubo(NULL),
	cPos(posInicial),
	cRotacao({0.0f, 0.0f, 0.0f}),
	cVel({ 0.0f, 0.0f, 0.0f }),
	cAcel({ 0.1f, 0.1f, 0.03f }),
	cAcelerando(false),
	cPosOlhando({ 0.0f, 0.0f, -4.0f, 0.0f }),
	cPosCamera({ 0.0f, 2.0f, 3.5f, 0.0f })
{
	cPCubo = new Cubo("Data\\Textures\\pooface.tex", 1.0f, 1.0f, 1.0f, *pDirectX);
}

Face::~Face()
{
	if (cPCubo)
	{
		delete cPCubo;
		cPCubo = NULL;
	}
}

void Face::Renderizar(DirectXC* pDirectX) const
{
	DirectX::XMFLOAT4X4 worldMatrix;

	DirectX::XMFLOAT4 rot = { cRotacao.x, cRotacao.y, cRotacao.z, 1.0f };

	DirectX::XMVECTOR rotacao = DirectX::XMLoadFloat4(&rot);

	DirectX::XMFLOAT4 poss = { cPos.x, cPos.y, cPos.z, 1.0f };

	DirectX::XMMATRIX mTransRot = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(rotacao), DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&poss)));
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

	cPCubo->Renderizar(worldMatrix, worldInvTranspose, worldViewProj, *pDirectX);
}

void Face::Atualizar()
{
	if (cPos.x + cVel.x < 50.0f)
	{
		cPos.x += cVel.x;
	}
	else
	{
		cPos.x = 50.0f;
	}

	if (cPos.x + cVel.x > 0.0f)
	{
		cPos.x += cVel.x;
	}
	else
	{
		cPos.x = 0.0f;
	}

	if (cPos.z + cVel.z < 50.0f)
	{
		cPos.z += cVel.z;
	}
	else
	{
		cPos.z = 50.0f;
	}

	if (cPos.z + cVel.z > 0.0f)
	{
		cPos.z += cVel.z;
	}
	else
	{
		cPos.z = 0.0f;
	}

	cVel.x *= 0.85f;
	cVel.y *= 0.85f;
	cVel.z *= 0.85f;
}

void Face::ApertouCima()
{
	DirectX::XMVECTOR vel = { 0.0f, 0.0f, cAcel.z, 0.0f };
	DirectX::XMVECTOR vRot;

	DirectX::XMFLOAT4 rot = { cRotacao.x, cRotacao.y, cRotacao.z, 1.0f };

	DirectX::XMVECTOR rotacao = DirectX::XMLoadFloat4(&rot);

	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYawFromVector(rotacao);

	vRot = DirectX::XMVector3Transform(vel, mRot);

	cVel.x -= vRot.m128_f32[0];
	cVel.y -= vRot.m128_f32[1];
	cVel.z -= vRot.m128_f32[2];
}

void Face::ApertouBaixo()
{
	DirectX::XMVECTOR vel = { 0.0f, 0.0f, cAcel.z, 0.0f };
	DirectX::XMVECTOR vRot;

	DirectX::XMFLOAT4 rot = { cRotacao.x, cRotacao.y, cRotacao.z, 1.0f };

	DirectX::XMVECTOR rotacao = DirectX::XMLoadFloat4(&rot);

	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYawFromVector(rotacao);

	vRot = DirectX::XMVector3Transform(vel, mRot);

	cVel.x += vRot.m128_f32[0];
	cVel.y += vRot.m128_f32[1];
	cVel.z += vRot.m128_f32[2];
}

void Face::ApertouDireita()
{
	cRotacao.y += 0.035f;
}

void Face::ApertouEsquerda()
{
	cRotacao.y -= 0.035f;
}

void Face::RodouBolinhaMouse(float d)
{
	cPosCamera.z += d;
}

void Face::MudouPosMouse(DirectX::XMFLOAT2 dPos)
{
	cPosOlhando.x += dPos.x;
	cPosOlhando.y += dPos.y;
}

void Face::PosicionarCamera(Camera* pCamera)
{
	DirectX::XMVECTOR posOlho = DirectX::XMLoadFloat4(&cPosCamera);
	DirectX::XMVECTOR poRot = { 0.0f, 0.0f, 0.0f, 0.0f };

	DirectX::XMFLOAT4 root = { cRotacao.x, cRotacao.y, cRotacao.z, 1.0f };

	DirectX::XMVECTOR rootacao = DirectX::XMLoadFloat4(&root);

	DirectX::XMMATRIX mmRot = DirectX::XMMatrixRotationRollPitchYawFromVector(rootacao);

	poRot = DirectX::XMVector3Transform(posOlho, mmRot);

	pCamera->SetarPosOlho({ poRot.m128_f32[0] + cPos.x, poRot.m128_f32[1] + cPos.y, poRot.m128_f32[2] + cPos.z, 1.0f });

	DirectX::XMVECTOR posOlhando = DirectX::XMLoadFloat4(&cPosOlhando);
	DirectX::XMVECTOR pRot = {0.0f, 0.0f, 0.0f, 0.0f};

	DirectX::XMFLOAT4 rot = { cRotacao.x, cRotacao.y, cRotacao.z, 1.0f };

	DirectX::XMVECTOR rotacao = DirectX::XMLoadFloat4(&rot);

	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYawFromVector(rotacao);

	pRot = DirectX::XMVector3Transform(posOlhando, mRot);

	pCamera->SetarPosOlhando({ pRot.m128_f32[0] + cPos.x, pRot.m128_f32[1] + cPos.y, pRot.m128_f32[2] + cPos.z, 1.0f });
}