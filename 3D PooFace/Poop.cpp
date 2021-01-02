#include "Poop.h"
#include "Face.h"

Poop::Poop(DirectX::XMFLOAT4 cPos, const Cubo* pCubo)
	:
	cPCubo(pCubo),
	cPos(cPos),
	cTempo((rand() % 255) / 255.0f)
{}

Poop::~Poop()
{}

void Poop::Renderizar(DirectXC* pDirectX) const
{
	DirectX::XMFLOAT4X4 worldMatrix;
	
	DirectX::XMFLOAT4 poss = { cPos.x, cPos.y, cPos.z, 1.0f };

	DirectX::XMMATRIX mTransRot = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&poss));
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

void Poop::Atualizar()
{
	cPos.y += 0.02f * cosf(cTempo);

	if (cTempo > 2.0f * Pi)
	{
		cTempo = 0.0f;
	}
	else
	{
		cTempo += 0.05f;
	}
}

bool Poop::TestarColisaoFace(const Face* pFace) const
{
	DirectX::XMVECTOR posFace = DirectX::XMLoadFloat3(&pFace->PegarPosFace());
	DirectX::XMVECTOR posPoo = DirectX::XMLoadFloat4(&cPos);

	DirectX::XMVECTOR d = DirectX::XMVectorSubtract(posFace, posPoo);
	float dist = DirectX::XMVector3LengthSq(d).m128_f32[0];

	if (dist <= powf(3.5f, 2.0f))
	{
		return true;
	}
	
	return false;
}