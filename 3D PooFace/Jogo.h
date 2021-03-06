#pragma once
#include "DirectXC.h"
#include "GameTimer.h"
#include "Camera.h"

#include "Teclado.h"
#include "Mouse.h"
#include "ControleXB.h"

#include <string>
#include <math.h>

#include "Face.h"
#include "Poop.h"
#include "Mapa.h"

#include "Fonte.h"

#include "Sound.h"

#include "MapaBlocos.h"

#include "PiperPA18.h"

class Jogo
{
public:
	Jogo(HWND hJnl, TecladoCliente& teclado, MouseClient& mouse, ControleXBCliente& joystick);
	~Jogo();

	void Vai();

	void MudouTamanhoJanela();

private:
	void Atualizar();
	void Renderizar();
	void Controles();

	void CalcularEstatisticasDeTempo();
private:
	DirectXC dxd;
	Camera cam;
	DSound audio;

	TecladoCliente& cTeclado;
	MouseClient& cMouse;
	ControleXBCliente& cJoystick;

	//Luz
	DirectionalLight cDirLight;
	PointLight cPointLight;
	SpotLight cSpotLight;

	GameTimer timer;

	Face cFace;

	Cubo cPoopCubo;

	PiperPA18 cPiper;

	MapaBlocos cMapaBlocos;

	Mapa cMapa;

	std::vector<Poop*> cBostas;

	bool cMousePrecionado;
	DirectX::XMFLOAT2 cPosMouseAnt;

	const UINT cNBostas;
	UINT cBostasComidas;

	Fonte cFFixedSys;

	TexturedQuad cImgGameOver;

	Sound cFart;
	Sound cTinkle;
};