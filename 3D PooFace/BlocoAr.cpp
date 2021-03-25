#include "BlocoAr.h"


BlocoAr::BlocoAr(DirectXC& dxd)
	:
	Bloco(L"Data\\Textures\\airblock.png", dxd)
{}

BlocoAr::~BlocoAr()
{}


bool BlocoAr::DeveRenderizar() const
{
	return false;
}