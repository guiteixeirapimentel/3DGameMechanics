#include "Cubo.h"

Cubo::Cubo(const std::string& nomeTextura, float altura, float largura, float profundidade, DirectXC& dxd)
{	
	LoadVertexStuff(altura, largura, profundidade, dxd);
	cPTextura = new Textura(dxd, nomeTextura);
}

Cubo::Cubo(const std::wstring& nomeTextura, float altura, float largura, float profundidade, DirectXC& dxd)
{
	LoadVertexStuff(altura, largura, profundidade, dxd);
	cPTextura = new Textura(dxd, nomeTextura);
}

void Cubo::LoadVertexStuff(float altura, float largura, float profundidade, DirectXC& dxd)
{
	//Vertex3D v[24];
	std::vector<Vertex3D> v;
	v.resize(24);
	float w2 = largura / 2.0f;
	float h2 = altura / 2.0f;
	float d2 = profundidade / 2.0f;

	// Fill in the front face vertex data.
	v[0] = { { -w2, -h2, -d2 },{ 0.0f, 0.0f, -1.0f },{ 0.25f, 1.0f } };
	v[1] = { { -w2, +h2, -d2 },{ 0.0f, 0.0f, -1.0f },{ 0.25f, 2.0f / 3.0f } };
	v[2] = { { +w2, +h2, -d2 },{ 0.0f, 0.0f, -1.0f },{ 0.5f, 2.0f / 3.0f } };
	v[3] = { { +w2, -h2, -d2 },{ 0.0f, 0.0f, -1.0f },{ 0.5f, 1.0f } };

	// Fill in the back face vertex data.
	v[4] = { { -w2, -h2, +d2 },{ 0.0f, 0.0f, 1.0f },{ 0.25f, 1.0f / 3.0f } };
	v[5] = { { +w2, -h2, +d2 },{ 0.0f, 0.0f, 1.0f },{ 0.25f, 0.0f } };
	v[6] = { { +w2, +h2, +d2 },{ 0.0f, 0.0f, 1.0f },{ 0.5f, 0.0f } };
	v[7] = { { -w2, +h2, +d2 },{ 0.0f, 0.0f, 1.0f },{ 0.5f, 1.0f / 3.0f } };

	// Fill in the top face vertex data.
	v[8] = { { -w2, +h2, -d2 },{ 0.0f, 1.0f, 0.0f },{ 0.25f, 2.0f / 3.0f } };
	v[9] = { { -w2, +h2, +d2 },{ 0.0f, 1.0f, 0.0f },{ 0.25f, 1.0f / 3.0f } };
	v[10] = { { +w2, +h2, +d2 },{ 0.0f, 1.0f, 0.0f },{ 0.5f, 1.0f / 3.0f } };
	v[11] = { { +w2, +h2, -d2 },{ 0.0f, 1.0f, 0.0f },{ 0.5f, 2.0f / 3.0f } };

	// Fill in the bottom face vertex data.
	v[12] = { { -w2, -h2, -d2 },{ 0.0f, -1.0f, 0.0f },{ 0.75f, 2.0f / 3.0f } };
	v[13] = { { +w2, -h2, -d2 },{ 0.0f, -1.0f, 0.0f },{ 0.75f, 1.0f / 3.0f } };
	v[14] = { { +w2, -h2, +d2 },{ 0.0f, -1.0f, 0.0f },{ 1.0f, 1.0f / 3.0f } };
	v[15] = { { -w2, -h2, +d2 },{ 0.0f, -1.0f, 0.0f },{ 1.0f, 2.0f / 3.0f } };

	// Fill in the left face vertex data.
	v[16] = { { -w2, -h2, +d2 },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f / 3.0f } };
	v[17] = { { -w2, +h2, +d2 },{ -1.0f, 0.0f, 0.0f },{ 0.25f, 1.0f / 3.0f } };
	v[18] = { { -w2, +h2, -d2 },{ -1.0f, 0.0f, 0.0f },{ 0.25f, 2.0f / 3.0f } };
	v[19] = { { -w2, -h2, -d2 },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 2.0f / 3.0f } };

	// Fill in the right face vertex data.
	v[20] = { { +w2, -h2, -d2 },{ 1.0f, 0.0f, 0.0f },{ 0.5f, 1.0f / 3.0f } };
	v[21] = { { +w2, +h2, -d2 },{ 1.0f, 0.0f, 0.0f },{ 0.75f, 1.0f / 3.0f } };
	v[22] = { { +w2, +h2, +d2 },{ 1.0f, 0.0f, 0.0f },{ 0.75f, 2.0f / 3.0f } };
	v[23] = { { +w2, -h2, +d2 },{ 1.0f, 0.0f, 0.0f },{ 0.5f, 2.0f / 3.0f } };

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(Vertex3D) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = v.data();
	HR(dxd.PegarPDevice()->CreateBuffer(&vertexBufferDesc, &vInitData, &cPVertexBuffer));

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	cIndexNum = 36;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = (UINT)(sizeof(UINT) * cIndexNum);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = i;
	HR(dxd.PegarPDevice()->CreateBuffer(&ibd, &iInitData, &cPIndexBuffer));

	cMaterial.Ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cMaterial.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cMaterial.Specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
}

Cubo::~Cubo()
{
	ReleaseCOM(cPVertexBuffer);
	ReleaseCOM(cPIndexBuffer);
}