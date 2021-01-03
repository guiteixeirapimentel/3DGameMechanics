#include "Textura.h"
#include "GDIPlusManager.h"

#include <gdiplus.h>

Textura::Textura(DirectXC& dxd, const std::string& nomeArqTextura)
	:
	cPTextura(nullptr)
{
	ID3D11Device *pDevice = dxd.PegarPDevice();
	UINT larguraTextura, alturaTextura;

	char *pPixels = nullptr;

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

Textura::Textura(DirectXC& dxd, const std::wstring& nomeArqTextura)
	:
	cPTextura(nullptr)
{
	ID3D11Device *pDevice = dxd.PegarPDevice();
	UINT larguraTextura, alturaTextura;

	char *pPixels = nullptr;

	LoadImageData(nomeArqTextura, &pPixels, &larguraTextura, &alturaTextura);

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

	ID3D11Texture2D *pTextura = nullptr;
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

void Textura::PegarDataImg(const std::string& nomeArq, char** pPixels, UINT *pLargura, UINT *pAltura)
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

void Textura::LoadImageData(const std::wstring& nomeArq, char** pPixels, UINT* pLargura, UINT* pAltura)
{
	GDIPlusManager gdiMan;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int pitch = 0;
	char* pBuffer = nullptr;

	Gdiplus::Bitmap bitmap(nomeArq.c_str());
	if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
	{
		assert(0);
	}

	width = bitmap.GetWidth();
	pitch = (width * 4) * sizeof(unsigned char);
	height = bitmap.GetHeight();
	pBuffer = new char[height * width * 4];

	*pLargura = width;
	*pAltura = height;

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			Gdiplus::Color c;
			bitmap.GetPixel(x, y, &c);
			const unsigned int color = c.GetB() << 16 | c.GetG() << 8  | c.GetR() << 0 | 0xFF000000;
			reinterpret_cast<unsigned int*>(pBuffer)[(y * width) + x] = color;

		}
	}

	*pPixels = pBuffer;

}