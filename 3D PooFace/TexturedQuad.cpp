#include "TexturedQuad.h"

TexturedQuad::TexturedQuad(std::string nomeImg, float largura, float altura, DirectX::XMFLOAT2 centerPos, DirectXC* pDirectX)
	:
	cStride(sizeof(Vertex2DTextured)),
	cOffset(0),
	cIndexNum(6),
	cPVertexBuffer(NULL),
	cPIndexBuffer(NULL)
{
	ID3D11Device *pDevice = pDirectX->PegarPDevice();

	//Cria textura
	UINT larguraTextura, alturaTextura;

	char *pPixels = NULL;

	std::ifstream img;
	img.open(nomeImg, std::ios::binary);

	if (img.is_open())
	{
		char c = 0;
		img >> c;
		img >> c;
		img >> c;
		img >> c;

		img >> alturaTextura;
		img >> c;
		img >> larguraTextura;
		img >> c;

		unsigned int nBytes = alturaTextura * (std::streamsize)larguraTextura * 4;

		pPixels = new char[nBytes];

		img.read(pPixels, nBytes);

		img.close();
	}
	else
	{
		MessageBox(NULL, L"FALTANDO ARQUIVOS!", L"REINSTALE", 0);
	}

	CriarBufferTextura(pPixels, larguraTextura, alturaTextura, pDirectX);
	delete[] pPixels;
	pPixels = NULL;

	//Cria Vertexes e Indexes	
	cLargura = largura;
	cAltura = altura;

	centerPos.x *= cLargura;
	centerPos.y *= cAltura;

	CriarQuadBuffers(centerPos, pDirectX);
}

TexturedQuad::TexturedQuad(std::string nomeImg, DirectX::XMFLOAT2 centerPos, DirectXC* pDirectX)
	:
	cStride(sizeof(Vertex2DTextured)),
	cOffset(0),
	cIndexNum(6),
	cPVertexBuffer(NULL),
	cPIndexBuffer(NULL)
{
	ID3D11Device *pDevice = pDirectX->PegarPDevice();

	//Cria textura
	UINT larguraTextura = 0;
	UINT alturaTextura = 0;

	char *pPixels = NULL;

	std::ifstream img;
	img.open(nomeImg, std::ios::binary);

	if (img.is_open())
	{
		char c = 0;
		img >> c;
		img >> c;
		img >> c;
		img >> c;

		img >> alturaTextura;
		img >> c;
		img >> larguraTextura;
		img >> c;

		unsigned int nBytes = alturaTextura * (std::streamsize)larguraTextura * 4;

		pPixels = new char[nBytes];

		img.read(pPixels, nBytes);

		img.close();
	}
	else
	{
		MessageBox(NULL, L"FALTANDO ARQUIVOS!", L"REINSTALE", 0);
	}

	CriarBufferTextura(pPixels, larguraTextura, alturaTextura, pDirectX);
	delete[] pPixels;
	pPixels = NULL;

	//Cria Vertexes e Indexes	
	cLargura = (float)larguraTextura;
	cAltura = (float)alturaTextura;

	centerPos.x *= cLargura;
	centerPos.y *= cAltura;

	CriarQuadBuffers(centerPos, pDirectX);
}

TexturedQuad::TexturedQuad(const void* pPixels, UINT larguraTextura, UINT alturaTextura,
	float larguraQuad, float alturaQuad, DirectX::XMFLOAT2 centerPos, DirectXC* pDirectX)
	:
	cStride(sizeof(Vertex2DTextured)),
	cOffset(0),
	cIndexNum(6),
	cPVertexBuffer(NULL),
	cPIndexBuffer(NULL)
{
	//Cria Vertexes e Indexes	
	cLargura = larguraQuad;
	cAltura = alturaQuad;

	centerPos.x *= cLargura;
	centerPos.y *= cAltura;

	CriarBufferTextura(pPixels, larguraTextura, alturaTextura, pDirectX);
	CriarQuadBuffers(centerPos, pDirectX);
}

void TexturedQuad::CriarQuadBuffers(DirectX::XMFLOAT2 centerPos, DirectXC* pDirectX)
{
	ID3D11Device *pDevice = pDirectX->PegarPDevice();

	const UINT numVertices = 4;
	Vertex2DTextured vertices[numVertices];

	vertices[0].pos = DirectX::XMFLOAT4(-centerPos.x, cAltura - centerPos.y, 0.0f, 1.0f);  // bottom left.
	vertices[0].textCoord = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	vertices[1].pos = DirectX::XMFLOAT4(-centerPos.x, -centerPos.y, 0.0f, 1.0f);  // top left.
	vertices[1].textCoord = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	vertices[2].pos = DirectX::XMFLOAT4(cLargura - centerPos.x, cAltura - centerPos.y, 0.0f, 1.0f);  // bottom right.
	vertices[2].textCoord = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f);

	vertices[3].pos = DirectX::XMFLOAT4(cLargura - centerPos.x, -centerPos.y, 0.0f, 1.0f);  // top right.
	vertices[3].textCoord = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(Vertex2DTextured) * numVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = vertices;
	pDevice->CreateBuffer(&vertexBufferDesc, &vInitData, &cPVertexBuffer);

	UINT indexes[6];
	/*
	1___3
	| \ |
	|  \|
	0---2
	*/

	//TRIANGULO DE CIMA		
	indexes[0] = 0;
	indexes[1] = 1;
	indexes[2] = 2;

	//TRIANGULO DE BAIXO
	indexes[3] = 2;
	indexes[4] = 1;
	indexes[5] = 3;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = (UINT)(sizeof(UINT) * cIndexNum);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = indexes;
	pDevice->CreateBuffer(&ibd, &iInitData, &cPIndexBuffer);
}

void TexturedQuad::CriarBufferTextura(const void* pPixelData, UINT larguraText, UINT alturaText, DirectXC* pDirectX)
{
	ID3D11Device *pDevice = pDirectX->PegarPDevice();

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = larguraText;
	desc.Height = alturaText;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pPixelData;
	initData.SysMemPitch = desc.Width * 4;
	initData.SysMemSlicePitch = desc.Width * desc.Height;

	ID3D11Texture2D *pTextura = NULL;
	pDevice->CreateTexture2D(&desc, &initData, &pTextura);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	pDevice->CreateShaderResourceView(pTextura, &srvDesc, &cPTextura);

	ReleaseCOM(pTextura);
}

TexturedQuad::~TexturedQuad()
{
	ReleaseCOM(cPVertexBuffer);
	ReleaseCOM(cPIndexBuffer);

	ReleaseCOM(cPTextura);
}

void TexturedQuad::Renderizar(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 escala, float rotacao, DirectXC* pDirectX) const
{
	DirectX::XMMATRIX matrixTranslation = DirectX::XMMatrixTranslation(pos.x, pos.y, 0.0f);
	DirectX::XMMATRIX matrixRotation = DirectX::XMMatrixRotationZ(rotacao);
	DirectX::XMMATRIX matrixScale = DirectX::XMMatrixScaling(escala.x, escala.y, 1.0f);

	DirectX::XMMATRIX worldMatrix;
	worldMatrix = DirectX::XMMatrixMultiply(matrixScale, matrixRotation);
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, matrixTranslation);

	ID3D11Device* pDevice = pDirectX->PegarPDevice();
	ID3D11DeviceContext* pDeviceContext = pDirectX->PegarPDeviceContext();

	pDirectX->SetarParametrosPorObjetoShader2D(&worldMatrix);

	pDeviceContext->PSSetShaderResources(0, 1, &cPTextura);

	//Seta buffers vertices e indexes
	pDeviceContext->IASetVertexBuffers(0, 1, &cPVertexBuffer, &cStride, &cOffset);
	pDeviceContext->IASetIndexBuffer(cPIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Desenha
	pDeviceContext->DrawIndexed(cIndexNum, 0, 0);
}
