#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>

#include "Renderizador.h"
#include "Renderizavel.h"

const float Pi = 3.1415926535f;

class Textura;
class Imagem;
class Modelo;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
		{													\
		HRESULT hr = (x);									\
		if(FAILED(hr))										\
				{											\
			LPWSTR output;									\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |			\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,				\
				NULL,										\
				hr,											\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,							\
				0,											\
				NULL);										\
			MessageBox(NULL, output, L"Erro!", MB_OK);		\
				}											\
		}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#ifndef ReleaseCOM
#define ReleaseCOM( x ) { if( x != NULL ) x->Release(); x = NULL; }
#endif

enum TIPO_CONSTANT_BUFFERS
{
	POR_OBJETO = 0,
	POR_FRAME = 1
};

struct Vertex3D
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 textCoord;
};

struct Vertex2DTextured
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 textCoord;
};

struct DirectionalLight
{
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT3 Direction;
	float pad;
};

struct PointLight
{
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;

	DirectX::XMFLOAT3 Position;
	float Range;

	DirectX::XMFLOAT3 Att;
	float pad;
};

struct SpotLight
{
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;

	DirectX::XMFLOAT3 Position;
	float Range;

	DirectX::XMFLOAT3 Direction;
	float Spot;

	DirectX::XMFLOAT3 Att;
	float pad;
};

struct Material
{
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular; // w = SpecPower
	DirectX::XMFLOAT4 Reflect;
};

struct ConstantBufferPorObjeto3D
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX worldViewProj;
	Material material;
};

struct ConstantBufferPorFrame3D
{
	DirectionalLight dirLight;
	PointLight pointLight;
	SpotLight spotLight;
	DirectX::XMFLOAT3 posOlhoW;
	float pad;
};

struct ConstantBufferPorObjeto2D
{
	DirectX::XMMATRIX worldMatrix;
};

class DirectXC : public Renderizador
{
public:
	DirectXC(HWND hJnl);
	~DirectXC();

	ID3D11Device* PegarPDevice() const;
	ID3D11DeviceContext* PegarPDeviceContext() const;

	void QuandoRedimensionar();

	void IniciarFrame();
	void FecharFrame();

	void SetarParametrosPorFrameShader3D(DirectionalLight cBDirLight, PointLight cBPointLight,
		SpotLight cBSpotLight, DirectX::XMFLOAT3 cBPosOlhoW);

	void SetarParametrosPorObjetoShader3D(DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 worldInvTranspose,
		DirectX::XMFLOAT4X4 worldViewProj, Material material, const Textura* textura);

	void SetarParametrosPorObjetoShader2D(const DirectX::XMMATRIX* worldMatrix);

	void Ligar3D();
	void Desligar3D();

	//void RenderizarModelo3D(Modelo* modelo);

	void Renderizar(Renderizavel* pRenderizavel) override;

	inline float PegarAspectoTela() const
	{
		return (float)cLargura / (float)cAltura;
	}

	float PegarAltura() const
	{
		return (float)cAltura;
	}

	float PegarLargura() const
	{
		return (float)cLargura;
	}

private:
	void CriarShaderEInputLayout();
	void CarregarDataDeArquivo(void** ppData, SIZE_T* pNBytesArq, std::string nomeArq);

private:
	UINT cLargura;
	UINT cAltura;

	HWND cHJnl;

	ID3D11Device			*cPDevice;
	ID3D11DeviceContext		*cPDeviceContext;
	IDXGISwapChain			*cPSwapChain;
	ID3D11Texture2D			*cPDepthStencilBuffer;
	ID3D11RenderTargetView	*cPRenderTargetView;
	ID3D11DepthStencilView	*cPDepthStencilView;
	D3D11_VIEWPORT			 cViewport;

	UINT cQualidadeMultisampleSuportado;

	bool cEnable4xMsaa;
	D3D_DRIVER_TYPE cDriverType;

	//SHADERS
	ID3D11VertexShader *cPVertexShader3D;
	ID3D11PixelShader  *cPPixelShader3D;

	ID3D11VertexShader *cPVertexShader2D;
	ID3D11PixelShader  *cPPixelShader2D;

	//INPUT LAYOUT
	ID3D11InputLayout *cPInputLayout3D;
	ID3D11InputLayout *cPInputLayout2D;

	ID3D11RasterizerState* cPRasterizerState;
	ID3D11BlendState*	   cPBlendState;

	//Depth Stencil State
	ID3D11DepthStencilState *cPDepthStencilState3D;
	ID3D11DepthStencilState *cPDepthStencilState2D;

	//Buffers constants(por objeto e por Frame)
	ID3D11Buffer *cPConstPorObjetoBuffer3D;
	ID3D11Buffer *cPConstPorFrameBuffer3D;

	ID3D11Buffer *cPConstPorObjetoBuffer2D;

	ID3D11SamplerState *cPSamplerState;

	DirectX::XMFLOAT4X4 cProjMatrix;

	DirectX::XMFLOAT4X4 cOrtographicMatrix;

	bool c3DLigado;
};