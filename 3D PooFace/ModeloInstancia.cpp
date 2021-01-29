#include "ModeloInstancia.h"

ModeloInstancia::ModeloInstancia(DirectXC& dxd, std::string nomeArqModelo, std::string nomeArqTextura)
	:
	cPIndexBuffer(NULL),
	cPVertexBuffer(NULL),
	cStride(sizeof(Vertex3D)),
	cOffset(NULL),
	cPTextura(NULL)
{
	std::vector<Vertex3D> verticesTemp;
	std::vector<UINT> indices;

	UINT indiceVertex = 0;

	std::vector<DirectX::XMFLOAT3> vertexData;
	std::vector<DirectX::XMFLOAT3> normalData;
	std::vector<DirectX::XMFLOAT2> textureData;

	std::ifstream modeloArq;
	modeloArq.open(nomeArqModelo);

	if (modeloArq.is_open())
	{
		char c;
		char c1;
		char buffer[512];

		while (!modeloArq.eof())
		{
			modeloArq.get(c);

			switch (c)
			{
			case '#':
			{
				//LINHA DE COMENTARIO
				modeloArq.getline(buffer, 512);
			}break;
			case 'f':
			{
				//TRIANGULO DATA (indice v/vt/vn)
				UINT indexVerticePos = 0;
				UINT indexVerticeNorm = 0;
				UINT indexVerticeTex = 0;
				for (int i = 0; i < 3; i++)
				{
					modeloArq >> indexVerticePos;
					modeloArq >> c;
					modeloArq >> indexVerticeTex;
					modeloArq >> c;
					modeloArq >> indexVerticeNorm;

					Vertex3D vertice;
					vertice.pos = vertexData[indexVerticePos - 1];
					vertice.normal = normalData[indexVerticeNorm - 1];
					vertice.textCoord = textureData[indexVerticeTex - 1];

					indices.push_back(indiceVertex);
					indiceVertex++;

					verticesTemp.push_back(vertice);
				}

			}break;
			case 'v':
			{
				//VERTICE ou NORMAL ou TEXTCOORD
				modeloArq.get(c1);
				switch (c1)
				{
				case ' ':
				{
					DirectX::XMFLOAT3 vert;
					modeloArq >> vert.x;
					modeloArq >> vert.y;
					modeloArq >> vert.z;

					vert.z = vert.z * -1.0f;

					modeloArq.get();

					vertexData.push_back(vert);
				}break;
				case 't':
				{
					DirectX::XMFLOAT2 tex;
					modeloArq >> tex.x;
					modeloArq >> tex.y;

					tex.y = 1.0f - tex.y;

					modeloArq.get();

					textureData.push_back(tex);
				}break;
				case 'n':
				{
					DirectX::XMFLOAT3 norm;
					modeloArq >> norm.x;
					modeloArq >> norm.y;
					modeloArq >> norm.z;

					norm.z = norm.z * -1.0f;

					modeloArq.get();

					normalData.push_back(norm);
				}break;
				}
			}break;
			case '\n':
			{
			}break;
			default:
			{
				modeloArq.getline(buffer, 512);

			}break;
			}
		}

		std::vector<Vertex3D> vertices;
		for (std::vector<Vertex3D>::reverse_iterator i = verticesTemp.rbegin();
			i != verticesTemp.rend(); i++)
		{
			Vertex3D v = *i;
			vertices.push_back(v);
		}

		cIndexNum = indices.size();

		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = UINT(sizeof(Vertex3D) * vertices.size());
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = vertices.data();
		HR(dxd.PegarPDevice()->CreateBuffer(&vertexBufferDesc, &vInitData, &cPVertexBuffer));

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = (UINT)(sizeof(UINT) * indices.size());
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iInitData;
		iInitData.pSysMem = indices.data();
		HR(dxd.PegarPDevice()->CreateBuffer(&ibd, &iInitData, &cPIndexBuffer));
	}

	cMaterial.Ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cMaterial.Diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cMaterial.Specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);

	cPTextura = new Textura(dxd, nomeArqTextura);
}

ModeloInstancia::ModeloInstancia()
	:
	cPIndexBuffer(NULL),
	cPVertexBuffer(NULL),
	cStride(sizeof(Vertex3D)),
	cOffset(NULL),
	cIndexNum(0)
{}

ModeloInstancia::~ModeloInstancia()
{
	ReleaseCOM(cPVertexBuffer);
	ReleaseCOM(cPIndexBuffer);

	if (cPTextura)
	{
		delete cPTextura;
		cPTextura = NULL;
	}
}

void ModeloInstancia::Renderizar(DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 worldInvTranspose, DirectX::XMFLOAT4X4 worldViewProj,
	DirectXC& dxd) const
{
	dxd.SetarParametrosPorObjetoShader3D(worldMatrix, worldInvTranspose, worldViewProj, cMaterial, cPTextura);

	ID3D11DeviceContext *deviceContext = dxd.PegarPDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &cPVertexBuffer, &cStride, &cOffset);
	deviceContext->IASetIndexBuffer(cPIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	constexpr UINT startIndexLocation = 0;
	constexpr UINT baseVertexLocation = 0;
	constexpr UINT instanceCount = 0;
	constexpr UINT startInstanceLocation = 0;

	//deviceContext->DrawIndexed((UINT)cIndexNum, startIndexLocation, baseVertexLocation);
	deviceContext->DrawIndexedInstanced((UINT)cIndexNum, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}