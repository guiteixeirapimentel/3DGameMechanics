#include "Camera.h"
#include "Renderizavel.h"

Camera::Camera(DirectX::XMFLOAT4 posInicial, DirectX::XMFLOAT4 rotacaoInicial, DirectX::XMFLOAT4 posOlhandoInicial,
	Renderizador& proximo)
	:
	cProximo(proximo),
	cPos(posInicial),
	cRotacao(rotacaoInicial),
	cPosOlhando(posOlhandoInicial)
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat4(&cPos);;
	DirectX::XMVECTOR posOlhando = DirectX::XMLoadFloat4(&cPosOlhando);
	DirectX::XMFLOAT4 cima(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR vetorCimaN = DirectX::XMLoadFloat4(&cima);

	DirectX::XMVECTOR rotacao = DirectX::XMLoadFloat4(&cRotacao);

	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(pos, posOlhando, vetorCimaN);

	DirectX::XMStoreFloat4x4(&cViewMatrix, viewMatrix);
}

void Camera::Atualizar()
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat4(&cPos);;
	DirectX::XMVECTOR posOlhando = DirectX::XMLoadFloat4(&cPosOlhando);
	DirectX::XMFLOAT4 cima(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR vetorCimaN = DirectX::XMLoadFloat4(&cima);

	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(pos, posOlhando, vetorCimaN);

	DirectX::XMStoreFloat4x4(&cViewMatrix, viewMatrix);
}

void Camera::Renderizar(Renderizavel* pRenderizavel)
{
	pRenderizavel->SetarViewMatrix(cViewMatrix);

	cProximo.Renderizar(pRenderizavel);
}

DirectX::XMFLOAT4 Camera::PegarPosOlho() const
{
	return cPos;
}

DirectX::XMFLOAT4 Camera::PegarPosOlhando() const
{
	return cPosOlhando;
}

void Camera::SetarPosOlhando(DirectX::XMFLOAT4 nPos)
{
	cPosOlhando = nPos;
}

void Camera::SetarPosOlho(DirectX::XMFLOAT4 nPos)
{
	cPos = nPos;
}