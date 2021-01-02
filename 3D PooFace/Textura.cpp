#include "Textura.h"

Textura::Textura(DirectXC& dxd, std::string nomeArqTextura)
	:
	cPTextura(NULL)
{
	ID3D11Device *pDevice = dxd.PegarPDevice();
	UINT larguraTextura, alturaTextura;

	char *pPixels = NULL;

	PegarDataImg(nomeArqTextura, &pPixels, &larguraTextura, &alturaTextura);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = larguraTextura;
	desc.Height = alturaTextura;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	//desc.

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = reinterpret_cast<void*>(pPixels);
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

	cLargura = larguraTextura;
	cAltura = alturaTextura;

	ReleaseCOM(pTextura);
	delete[] pPixels;
}

Textura::~Textura()
{
	ReleaseCOM(cPTextura);
}

ID3D11ShaderResourceView *Textura::PegarTextura() const
{
	return cPTextura;
}

UINT Textura::PegarAltura() const
{
	return cAltura;
}

UINT Textura::PegarLargura() const
{
	return cLargura;
}

void Textura::PegarDataImg(std::string nomeArq, char** pPixels, UINT *pLargura, UINT *pAltura)
{
	std::ifstream img;
	img.open(nomeArq, std::ios::binary);

	if (img.is_open())
	{
		char c = 0;
		img >> c;
		img >> c;
		img >> c;
		img >> c;

		img >> *pAltura;
		img >> c;
		img >> *pLargura;
		img >> c;

		UINT alturaTextura = *pAltura;
		UINT larguraTextura = *pLargura;

		unsigned int nBytes = alturaTextura * (std::streamsize)larguraTextura * 4;

		*pPixels = new char[nBytes];

		img.read(*pPixels, nBytes);

		img.close();
	}
	else
	{
		MessageBox(NULL, L"FALTANDO ARQUIVOS!", L"REINSTALE", 0);
	}
}