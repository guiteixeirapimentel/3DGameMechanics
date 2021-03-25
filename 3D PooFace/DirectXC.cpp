#include "DirectXC.h"
#include "Modelo.h"
#include "Textura.h"

DirectXC::DirectXC(HWND hJnl)
	:
	cHJnl(hJnl),
	cLargura(0),
	cAltura(0),
	cPDepthStencilBuffer(NULL),
	cPDepthStencilView(NULL),
	cPDevice(NULL),
	cPDeviceContext(NULL),
	cPRenderTargetView(NULL),
	cPSwapChain(NULL),
	cQualidadeMultisampleSuportado(0),
	cDriverType(D3D_DRIVER_TYPE_HARDWARE),
	cEnable4xMsaa(true),
	cPVertexShader3D(NULL),
	cPPixelShader3D(NULL),
	cPBlendState(NULL),
	c3DLigado(true)
{
	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	if (FAILED(D3D11CreateDevice(NULL, cDriverType, NULL, flags, NULL, 0, D3D11_SDK_VERSION, &cPDevice, &featureLevel, &cPDeviceContext)))
	{
		MessageBox(NULL, L"Erro ao criar device!", L"Erro", MB_ICONERROR);
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(NULL, L"Directx 11 não é suportado!", L"AVISO", MB_ICONEXCLAMATION);
	}

	HR(cPDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &cQualidadeMultisampleSuportado));

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = cLargura;
	sd.BufferDesc.Height = cAltura;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (cEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = cQualidadeMultisampleSuportado - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hJnl;
	sd.BufferCount = 1;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice *dxgiDevice = NULL;
	HR(cPDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(cPDevice, &sd, &cPSwapChain));

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	QuandoRedimensionar();

	CriarShaderEInputLayout();

	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.DepthClipEnable = true;

	cPDevice->CreateRasterizerState(&rastDesc, &cPRasterizerState);
	
	D3D11_SAMPLER_DESC samplerDesc;
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	cPDevice->CreateSamplerState(&samplerDesc, &cPSamplerState);

	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	// Create an alpha enabled blend state description.
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	cPDevice->CreateBlendState(&blendDesc, &cPBlendState);

	//Cria Depth Stencil STATE para ativar/desativar 3D
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 3D
	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Create the depth stencil state.
	cPDevice->CreateDepthStencilState(&depthStencilDesc, &cPDepthStencilState3D);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//2D
	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	cPDevice->CreateDepthStencilState(&depthStencilDesc, &cPDepthStencilState2D);
}

DirectXC::~DirectXC()
{
	if (cPDeviceContext)
	{
		cPDeviceContext->ClearState();
		cPDeviceContext->OMSetRenderTargets(0, 0, 0);
	}

	if (cPSwapChain)
	{
		cPSwapChain->SetFullscreenState(FALSE, NULL);
	}

	//Depth Stencil State
	ReleaseCOM(cPDepthStencilState3D);
	ReleaseCOM(cPDepthStencilState2D);

	//INPUT LAYOUT
	ReleaseCOM(cPInputLayout3D);
	ReleaseCOM(cPInputLayout2D);
	ReleaseCOM(cPRasterizerState);
	ReleaseCOM(cPBlendState);

	//Buffers constantes
	ReleaseCOM(cPConstPorFrameBuffer3D);
	ReleaseCOM(cPConstPorObjetoBuffer3D);
	ReleaseCOM(cPConstPorObjetoBuffer2D);

	//SHADERS
	ReleaseCOM(cPVertexShader3D);
	ReleaseCOM(cPPixelShader3D);
	ReleaseCOM(cPVertexShader2D);
	ReleaseCOM(cPPixelShader2D);

	ReleaseCOM(cPDepthStencilBuffer);
	ReleaseCOM(cPDepthStencilView);
	ReleaseCOM(cPRenderTargetView);
	ReleaseCOM(cPSwapChain);

	ReleaseCOM(cPSamplerState);

	ReleaseCOM(cPDeviceContext);
	ReleaseCOM(cPDevice);
}

void DirectXC::QuandoRedimensionar()
{
	RECT ret;
	GetClientRect(cHJnl, &ret);

	cLargura = static_cast<UINT>(ret.right - ret.left);
	cAltura = static_cast<UINT>(ret.bottom - ret.top);

	if (cLargura < 10 || cAltura < 10)
	{
		cLargura = 10;
		cAltura = 10;
	}

	cPDeviceContext->OMSetRenderTargets(0, 0, 0);

	ReleaseCOM(cPRenderTargetView);
	ReleaseCOM(cPDepthStencilBuffer);
	ReleaseCOM(cPDepthStencilView);

	HR(cPSwapChain->ResizeBuffers(1, cLargura, cAltura, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D11Texture2D *backBuffer;
	HR(cPSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(cPDevice->CreateRenderTargetView(backBuffer, 0, &cPRenderTargetView));
	ReleaseCOM(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = cLargura;
	depthStencilDesc.Height = cAltura;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (cEnable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = cQualidadeMultisampleSuportado - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(cPDevice->CreateTexture2D(&depthStencilDesc, 0, &cPDepthStencilBuffer));
	HR(cPDevice->CreateDepthStencilView(cPDepthStencilBuffer, 0, &cPDepthStencilView));

	cPDeviceContext->OMSetRenderTargets(1, &cPRenderTargetView, cPDepthStencilView);

	cViewport.Height = static_cast<float>(cAltura);
	cViewport.Width = static_cast<float>(cLargura);
	cViewport.MaxDepth = 1.0f;
	cViewport.MinDepth = 0.0f;
	cViewport.TopLeftX = 0;
	cViewport.TopLeftY = 0;

	cPDeviceContext->RSSetViewports(1, &cViewport);

	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * Pi, PegarAspectoTela(), 1.0f, D3D11_FLOAT32_MAX);
	DirectX::XMStoreFloat4x4(&cProjMatrix, projMatrix);

	DirectX::XMMATRIX orthoMatrix = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, (float)cLargura, (float)cAltura, 0.0f, 0.0f, 100.0f);
	DirectX::XMStoreFloat4x4(&cOrtographicMatrix, orthoMatrix);
}

void DirectXC::IniciarFrame()
{
	float corFundo[4];
	corFundo[0] = 0.1f;
	corFundo[1] = 0.1f;
	corFundo[2] = 0.1f;
	corFundo[3] = 1.0f;
	
	cPDeviceContext->RSSetState(cPRasterizerState);

	cPDeviceContext->ClearRenderTargetView(cPRenderTargetView, corFundo);
	cPDeviceContext->ClearDepthStencilView(cPDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	cPDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	float blendFactors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	cPDeviceContext->OMSetBlendState(cPBlendState, blendFactors, 0xffffffff);
}

void DirectXC::FecharFrame()
{
	HR(cPSwapChain->Present(1, 0));
}

void DirectXC::Ligar3D()
{
	c3DLigado = true;
	
	cPDeviceContext->VSSetShader(cPVertexShader3D, NULL, 0);
	cPDeviceContext->PSSetShader(cPPixelShader3D, NULL, 0);
	cPDeviceContext->IASetInputLayout(cPInputLayout3D);
	cPDeviceContext->OMSetDepthStencilState(cPDepthStencilState3D, 1);
	cPDeviceContext->PSSetSamplers(0, 1, &cPSamplerState);
}

void DirectXC::Desligar3D()
{
	c3DLigado = false;
	
	cPDeviceContext->VSSetShader(cPVertexShader2D, NULL, 0);
	cPDeviceContext->PSSetShader(cPPixelShader2D, NULL, 0);
	cPDeviceContext->IASetInputLayout(cPInputLayout2D);
	cPDeviceContext->OMSetDepthStencilState(cPDepthStencilState2D, 1);
	cPDeviceContext->PSSetSamplers(0, 1, &cPSamplerState);
}

ID3D11Device* DirectXC::PegarPDevice() const
{
	return cPDevice;
}

ID3D11DeviceContext* DirectXC::PegarPDeviceContext() const
{
	return cPDeviceContext;
}

void DirectXC::CriarShaderEInputLayout()
{
	ID3D10Blob *mensagemErro = NULL;
	void* pCompiled3DVertexShaderBuffer = NULL;
	void* pCompiled3DPixelShaderBuffer = NULL;

	SIZE_T nBytesCompiled3DVertexShaderBuffer = 0;
	SIZE_T nBytesCompiled3DPixelShaderBuffer = 0;

	void* pCompiled2DVertexShaderBuffer = NULL;
	void* pCompiled2DPixelShaderBuffer = NULL;

	SIZE_T nBytesCompiled2DVertexShaderBuffer = 0;
	SIZE_T nBytesCompiled2DPixelShaderBuffer = 0;

#if (_DEBUG || DEBUG) || 1

	ID3D10Blob *vertexShaderBuffer3D = NULL;
	ID3D10Blob *pixelShaderBuffer3D = NULL;

	UINT flags1 = 0;
	//flags1 |= D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0;

	//Compila Vertex Shader 3D
	HRESULT res = D3DCompileFromFile(L"3DVertexShader.hlsl", NULL, NULL, "VS", "vs_5_0", flags1, 0, &vertexShaderBuffer3D, &mensagemErro);

	if (FAILED(res))
	{
		if (mensagemErro)
		{
			MessageBoxA(NULL, (char*)mensagemErro->GetBufferPointer(), "Erro ao compilar Vertex Shader!", 0);
		}
		else
		{
			MessageBoxA(NULL, "3D Vertex shader nao encontrado!", "Erro!", 0);
		}
	}

	//Compila Pixel Shader 3D
	res = D3DCompileFromFile(L"3DPixelShader.hlsl", NULL, NULL, "PS", "ps_5_0", flags1, 0, &pixelShaderBuffer3D, &mensagemErro);

	if (FAILED(res))
	{
		if (mensagemErro)
		{
			MessageBoxA(NULL, (char*)mensagemErro->GetBufferPointer(), "Erro ao compilar PIXEL Shader!", 0);
		}
		else
		{
			MessageBoxA(NULL, "Pixel shader nao encontrado!", "Erro!", 0);
		}
	}


	pCompiled3DPixelShaderBuffer = pixelShaderBuffer3D->GetBufferPointer();
	nBytesCompiled3DPixelShaderBuffer = pixelShaderBuffer3D->GetBufferSize();

	pCompiled3DVertexShaderBuffer = vertexShaderBuffer3D->GetBufferPointer();
	nBytesCompiled3DVertexShaderBuffer = vertexShaderBuffer3D->GetBufferSize();
#else
	CarregarDataDeArquivo(&pCompiled3DPixelShaderBuffer, &nBytesCompiled3DPixelShaderBuffer, "Data\\Shaders\\3DPixelShader.cso");
	CarregarDataDeArquivo(&pCompiled3DVertexShaderBuffer, &nBytesCompiled3DVertexShaderBuffer, "Data\\Shaders\\3DVertexShader.cso");
#endif

	cPDevice->CreateVertexShader(pCompiled3DVertexShaderBuffer, nBytesCompiled3DVertexShaderBuffer, NULL, &cPVertexShader3D);
	cPDevice->CreatePixelShader(pCompiled3DPixelShaderBuffer, nBytesCompiled3DPixelShaderBuffer, NULL, &cPPixelShader3D);

	//Cria input layout 3D
	D3D11_INPUT_ELEMENT_DESC inputLayout[3];
	unsigned int numElements;

	inputLayout[0].SemanticName = "POSICAO";
	inputLayout[0].SemanticIndex = 0;
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[0].InputSlot = 0;
	inputLayout[0].AlignedByteOffset = 0;
	inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[0].InstanceDataStepRate = 0;

	inputLayout[1].SemanticName = "NORMAL";
	inputLayout[1].SemanticIndex = 0;
	inputLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[1].InputSlot = 0;
	inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[1].InstanceDataStepRate = 0;

	inputLayout[2].SemanticName = "TEXTCOORD";
	inputLayout[2].SemanticIndex = 0;
	inputLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayout[2].InputSlot = 0;
	inputLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(inputLayout) / sizeof(inputLayout[0]);

	cPDevice->CreateInputLayout(inputLayout, numElements, pCompiled3DVertexShaderBuffer,
		nBytesCompiled3DVertexShaderBuffer, &cPInputLayout3D);

#if (_DEBUG || DEBUG) || 1
	ReleaseCOM(vertexShaderBuffer3D);
	ReleaseCOM(pixelShaderBuffer3D);
#else
	delete pCompiled3DPixelShaderBuffer;
	delete pCompiled3DVertexShaderBuffer;

	pCompiled3DPixelShaderBuffer = NULL;
	pCompiled3DVertexShaderBuffer = NULL;
#endif

	//Cria constant buffers 3D
	D3D11_BUFFER_DESC constBufferPorObjetoDesc;
	constBufferPorObjetoDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferPorObjetoDesc.ByteWidth = sizeof(ConstantBufferPorObjeto3D);
	constBufferPorObjetoDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferPorObjetoDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferPorObjetoDesc.MiscFlags = 0;
	constBufferPorObjetoDesc.StructureByteStride = 0;

	cPDevice->CreateBuffer(&constBufferPorObjetoDesc, NULL, &cPConstPorObjetoBuffer3D);

	D3D11_BUFFER_DESC constBufferPorFrameDesc;
	constBufferPorFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferPorFrameDesc.ByteWidth = sizeof(ConstantBufferPorFrame3D);
	constBufferPorFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferPorFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferPorFrameDesc.MiscFlags = 0;
	constBufferPorFrameDesc.StructureByteStride = 0;

	cPDevice->CreateBuffer(&constBufferPorFrameDesc, NULL, &cPConstPorFrameBuffer3D);

#if (_DEBUG || DEBUG) || 1

	ID3D10Blob *vertexShaderBuffer2D = NULL;
	ID3D10Blob *pixelShaderBuffer2D = NULL;

	//Compila vertex shader 2D
	res = D3DCompileFromFile(L"2DVertexShader.hlsl", NULL, NULL, "VS", "vs_5_0", flags1, 0, &vertexShaderBuffer2D, &mensagemErro);

	if (FAILED(res))
	{
		if (mensagemErro)
		{
			MessageBoxA(NULL, (char*)mensagemErro->GetBufferPointer(), "Erro ao compilar Vertex Shader 2D!", 0);
		}
		else
		{
			MessageBoxA(NULL, "Vertex shader 2D nao encontrado!", "Erro!", 0);
		}
	}

	//Compila Pixel Shader 2D
	res = D3DCompileFromFile(L"2DPixelShader.hlsl", NULL, NULL, "PS", "ps_5_0", flags1, 0, &pixelShaderBuffer2D, &mensagemErro);

	if (FAILED(res))
	{
		if (mensagemErro)
		{
			MessageBoxA(NULL, (char*)mensagemErro->GetBufferPointer(), "Erro ao compilar PIXEL Shader 2D!", 0);
		}
		else
		{
			MessageBoxA(NULL, "Pixel shader 2D nao encontrado!", "Erro!", 0);
		}
	}
	pCompiled2DPixelShaderBuffer = pixelShaderBuffer2D->GetBufferPointer();
	pCompiled2DVertexShaderBuffer = vertexShaderBuffer2D->GetBufferPointer();

	nBytesCompiled2DPixelShaderBuffer = pixelShaderBuffer2D->GetBufferSize();
	nBytesCompiled2DVertexShaderBuffer = vertexShaderBuffer2D->GetBufferSize();
#else
	CarregarDataDeArquivo(&pCompiled2DPixelShaderBuffer, &nBytesCompiled2DPixelShaderBuffer, "Data\\Shaders\\2DPixelShader.cso");
	CarregarDataDeArquivo(&pCompiled2DVertexShaderBuffer, &nBytesCompiled2DVertexShaderBuffer, "Data\\Shaders\\2DVertexShader.cso");
#endif
	cPDevice->CreateVertexShader(pCompiled2DVertexShaderBuffer, nBytesCompiled2DVertexShaderBuffer, NULL, &cPVertexShader2D);
	cPDevice->CreatePixelShader(pCompiled2DPixelShaderBuffer, nBytesCompiled2DPixelShaderBuffer, NULL, &cPPixelShader2D);

	//Cria input layout 2D
	D3D11_INPUT_ELEMENT_DESC inputLayout2D[2];

	inputLayout2D[0].SemanticName = "POSICAO";
	inputLayout2D[0].SemanticIndex = 0;
	inputLayout2D[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout2D[0].InputSlot = 0;
	inputLayout2D[0].AlignedByteOffset = 0;
	inputLayout2D[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout2D[0].InstanceDataStepRate = 0;

	inputLayout2D[1].SemanticName = "TEXTCOORD";
	inputLayout2D[1].SemanticIndex = 0;
	inputLayout2D[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout2D[1].InputSlot = 0;
	inputLayout2D[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout2D[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout2D[1].InstanceDataStepRate = 0;

	numElements = sizeof(inputLayout2D) / sizeof(inputLayout2D[0]);

	cPDevice->CreateInputLayout(inputLayout2D, numElements, pCompiled2DVertexShaderBuffer,
		nBytesCompiled2DVertexShaderBuffer, &cPInputLayout2D);

#if (_DEBUG || DEBUG) || 1
	ReleaseCOM(vertexShaderBuffer2D);
	ReleaseCOM(pixelShaderBuffer2D);
#else
	delete pCompiled2DPixelShaderBuffer;
	delete pCompiled2DVertexShaderBuffer;

	pCompiled2DPixelShaderBuffer = NULL;
	pCompiled2DVertexShaderBuffer = NULL;
#endif

	//Cria constant buffers 2D
	constBufferPorObjetoDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferPorObjetoDesc.ByteWidth = sizeof(ConstantBufferPorObjeto2D);
	constBufferPorObjetoDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferPorObjetoDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferPorObjetoDesc.MiscFlags = 0;
	constBufferPorObjetoDesc.StructureByteStride = 0;

	cPDevice->CreateBuffer(&constBufferPorObjetoDesc, NULL, &cPConstPorObjetoBuffer2D);
}

void DirectXC::SetarParametrosPorFrameShader3D(DirectionalLight cBDirLight, PointLight cBPointLight,
	SpotLight cBSpotLight, DirectX::XMFLOAT3 cBPosOlhoW)
{
	ConstantBufferPorFrame3D data;
	data.dirLight = cBDirLight;
	data.pointLight = cBPointLight;
	data.spotLight = cBSpotLight;
	data.posOlhoW = cBPosOlhoW;
	data.pad = 1.0f;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	cPDeviceContext->Map(cPConstPorFrameBuffer3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	ConstantBufferPorFrame3D *dataPtr = reinterpret_cast<ConstantBufferPorFrame3D*>(mappedResource.pData);

	*dataPtr = data;

	//memset(dataPtr, 1, sizeof(ConstantBufferPorFrame));

	cPDeviceContext->Unmap(cPConstPorFrameBuffer3D, 0);

	cPDeviceContext->PSSetConstantBuffers(POR_FRAME, 1, &cPConstPorFrameBuffer3D);
}

void DirectXC::SetarParametrosPorObjetoShader3D(DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 worldInvTranspose,
	DirectX::XMFLOAT4X4 worldViewProj, Material material, const Textura* textura)
{
	DirectX::XMMATRIX wvp = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&worldViewProj));
	ConstantBufferPorObjeto3D cb;
	cb.worldInvTranspose = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&worldInvTranspose));
	cb.worldMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&worldMatrix));
	cb.worldViewProj = wvp;
	cb.material = material;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	cPDeviceContext->Map(cPConstPorObjetoBuffer3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	ConstantBufferPorObjeto3D *dataPtr = reinterpret_cast<ConstantBufferPorObjeto3D*>(mappedResource.pData);

	(*dataPtr) = cb;

	cPDeviceContext->Unmap(cPConstPorObjetoBuffer3D, 0);

	cPDeviceContext->PSSetConstantBuffers(POR_OBJETO, 1, &cPConstPorObjetoBuffer3D);
	cPDeviceContext->VSSetConstantBuffers(POR_OBJETO, 1, &cPConstPorObjetoBuffer3D);

	ID3D11ShaderResourceView *pTextura = textura->PegarTextura();
	cPDeviceContext->PSSetShaderResources(0, 1, &pTextura);
}

void DirectXC::SetarParametrosPorObjetoShader2D(const DirectX::XMMATRIX* worldMatrix)
{
	ConstantBufferPorObjeto2D buffer;
	buffer.worldMatrix = DirectX::XMMatrixMultiplyTranspose(*worldMatrix, DirectX::XMLoadFloat4x4(&cOrtographicMatrix));

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	cPDeviceContext->Map(cPConstPorObjetoBuffer2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	ConstantBufferPorObjeto2D *dataPtr = reinterpret_cast<ConstantBufferPorObjeto2D*>(mappedResource.pData);

	(*dataPtr) = buffer;

	cPDeviceContext->Unmap(cPConstPorObjetoBuffer2D, 0);
	cPDeviceContext->VSSetConstantBuffers(POR_OBJETO, 1, &cPConstPorObjetoBuffer2D);
}


void DirectXC::Renderizar(Renderizavel* pRenderizavel)
{
	pRenderizavel->SetarProjectionMatrix(cProjMatrix);
	pRenderizavel->Renderizar(this);
}

void DirectXC::CarregarDataDeArquivo(void** pData, SIZE_T* pNBytesArq, std::string nomeArq)
{
	std::ifstream arq(nomeArq, std::ios::binary);

	if (!arq.is_open())
	{
		std::string msg = "Erro ao abrir o arquivo: " + nomeArq;
		MessageBoxA(NULL, msg.c_str(), "Por favor Reinstale!", MB_ICONERROR);
	}

	arq.seekg(0, arq.end);
	SIZE_T tamanhoArq = (SIZE_T)arq.tellg();
	arq.seekg(0, arq.beg);

	*pData = reinterpret_cast<void*>(new char[tamanhoArq]);

	arq.read(reinterpret_cast<char*>(*pData), tamanhoArq);

	*pNBytesArq = tamanhoArq;
}
