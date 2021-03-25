#pragma once
#include "Bloco.h"

class BlocoAr : public Bloco
{
public:
	BlocoAr(DirectXC& dxd);
	~BlocoAr();

	bool DeveRenderizar() const override;

private:

};