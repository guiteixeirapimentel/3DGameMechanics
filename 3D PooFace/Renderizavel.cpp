#include "Renderizavel.h"

Renderizavel::Renderizavel()
{}

Renderizavel::~Renderizavel()
{

}
void Renderizavel::SetarProjectionMatrix(DirectX::XMFLOAT4X4 projMatrix)
{
	cProjMatrix = projMatrix;
}

void Renderizavel::SetarViewMatrix(DirectX::XMFLOAT4X4 viewMatrix)
{
	cViewMatrix = viewMatrix;
}