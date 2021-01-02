#include "Chao.h"

Chao::Chao(float largura, float comprimento, std::string nomeTextura, DirectXC& dxd)
{
	std::vector<Vertex3D> v;
	//v.resize(UINT(largura * comprimento) * 4);
	
	const float largQuad = ((largura - int(largura)) / largura) + 1.0f;
	const float compQuad = ((comprimento - int(comprimento)) / comprimento) + 1.0f;

	const float largQuad2 = largQuad / 2.0f;
	const float compQuad2 = compQuad / 2.0f;

	std::vector<UINT> i;
	//i.resize(UINT(largura * comprimento) * 6);

	for (UINT ix = 0; ix < (UINT)largura; ix++)
	{
		for (UINT iz = 0; iz < (UINT)comprimento; iz++)
		{
			Vertex3D v1, v2, v3, v4;

			v1.pos = { -largQuad2 + (1.0f * ix), -1.0f, -compQuad2 + (1.0f * iz) };
			v2.pos = { -largQuad2 + (1.0f * ix), -1.0f, compQuad2 + (1.0f * iz) };
			v3.pos = { largQuad2 + (1.0f * ix), -1.0f, compQuad2 + (1.0f * iz )};
			v4.pos = { largQuad2 + (1.0f * ix), -1.0f, -compQuad2 + (1.0f * iz) };

			v1.normal = { 0.0f, 1.0f, 0.0f };
			v2.normal = { 0.0f, 1.0f, 0.0f };
			v3.normal = { 0.0f, 1.0f, 0.0f };
			v4.normal = { 0.0f, 1.0f, 0.0f };

			v1.textCoord = { 0.0f, 1.0f };
			v2.textCoord = { 0.0f, 0.0f };
			v3.textCoord = { 1.0f, 1.0f };
			v4.textCoord = { 1.0f, 0.0f };

			v.push_back(v1);
			v.push_back(v2);
			v.push_back(v3);
			v.push_back(v4);

			const UINT iInicial = UINT(v.size() - 4);

			i.push_back(iInicial);
			i.push_back(iInicial + 1);
			i.push_back(iInicial + 2);

			i.push_back(iInicial);
			i.push_back(iInicial + 2);
			i.push_back(iInicial + 3);
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = UINT(sizeof(Vertex3D) * v.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = v.data();
	HR(dxd.PegarPDevice()->CreateBuffer(&vertexBufferDesc, &vInitData, &cPVertexBuffer));

	cIndexNum = i.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = (UINT)(sizeof(UINT) * cIndexNum);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = i.data();
	HR(dxd.PegarPDevice()->CreateBuffer(&ibd, &iInitData, &cPIndexBuffer));

	cMaterial.Ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cMaterial.Diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cMaterial.Specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 32.0f);

	cPTextura = new Textura(dxd, nomeTextura);
}

Chao::~Chao()
{}