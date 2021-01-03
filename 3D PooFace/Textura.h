#pragma once
#include "DirectXC.h"
#include <string>
#include <fstream>

class Textura
{
public:
	// Using ".TEX" file type.
	Textura(DirectXC& dxd, const std::string& nomeArqTextura);
	// Using GDI to get imagem (png, jpeg, etc)
	Textura(DirectXC& dxd, const std::wstring& nomeArqTextura);

	~Textura();

	ID3D11ShaderResourceView *PegarTextura() const;

	UINT PegarLargura() const;
	UINT PegarAltura() const;

private:
	void PegarDataImg(const std::string& nomeArq, char **data, UINT *largura, UINT *altura);
	void LoadImageData(const std::wstring& nomeArq, char **data, UINT *largura, UINT *altura);
private:
	ID3D11ShaderResourceView* cPTextura;

	UINT cLargura;
	UINT cAltura;
};