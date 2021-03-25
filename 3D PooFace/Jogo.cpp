#include "Jogo.h"

Jogo::Jogo(HWND hJnl, TecladoCliente& teclado, MouseClient& mouse, ControleXBCliente& joystick)
	:
	dxd(hJnl),
	audio(hJnl),
	cTeclado(teclado),
	cMouse(mouse),
	cJoystick(joystick),
	cam({ 0.0f, 0.0f, -5.0f, 1.0f }, { 0.0f , 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, dxd),
	cFace({ 0.0f, 0.0f, 0.0f }, &dxd),
	cPoopCubo(L"Data\\Textures\\boxTexture.png", 2.5f, 2.5f, 2.5f, dxd),
	cMapa(50.0f, 50.0f, &dxd),
	cMousePrecionado(false),
	cPosMouseAnt({ 0.0f, 0.0f }),
	cFFixedSys("Data\\Fonts\\Consolas13x24L.tex", 13, 24, &dxd),
	cImgGameOver("Data\\Textures\\GameOver.tex", dxd.PegarLargura() / 2.0f, dxd.PegarAltura() / 2.0f, { 0.5f, 0.5f }, &dxd),
	cNBostas(100),
	cBostasComidas(0),
	cPiper(dxd, {0.0f, 0.0f, 0.0f, 0.0f}),
	cMapaBlocos(20, 20, 20, &dxd) // http://www.rastertek.com/dx11tut37.html
{
	cFart = audio.CreateSound("Data\\Sounds\\farty.wav");
	cTinkle = audio.CreateSound("Data\\Sounds\\tinkle.wav");

	// Directional light.
	cDirLight.Ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	cDirLight.Diffuse = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	cDirLight.Specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	cDirLight.Direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Point light--position is changed every frame to animate in UpdateScene function.
	cPointLight.Ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	cPointLight.Diffuse = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	cPointLight.Specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	cPointLight.Att = DirectX::XMFLOAT3(0.0f, 0.1f, 0.0f);
	cPointLight.Position = DirectX::XMFLOAT3(0.0f, 0.1f, 0.0f);
	cPointLight.Range = 10.0f;

	// Spot light--position and direction changed every frame to animate in UpdateScene function.
	cSpotLight.Ambient = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	cSpotLight.Diffuse = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cSpotLight.Specular = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 32.0f);
	cSpotLight.Att = DirectX::XMFLOAT3(0.0f, 0.1f, 0.1f);
	cSpotLight.Spot = 128.0f;
	cSpotLight.Range = 1000.0f;

	for (UINT i = 0; i < cNBostas; i++)
	{
		Poop* p = new Poop({ float(rand() % 50), 1.0f, float(rand() % 50), 1.0f }, &cPoopCubo);
		cBostas.push_back(p);
	}
}

Jogo::~Jogo()
{
	for (UINT i = 0; i < cBostas.size(); i++)
	{
		delete cBostas[i];
		cBostas[i] = NULL;
	}
	cBostas.clear();
}

void Jogo::Vai()
{
	timer.Tick();

	Controles();

	Atualizar();

	dxd.IniciarFrame();
	Renderizar();
	dxd.FecharFrame();

	CalcularEstatisticasDeTempo();
}

void Jogo::Atualizar()
{
	DirectX::XMFLOAT4 posCam = cam.PegarPosOlho();
	cSpotLight.Position = DirectX::XMFLOAT3(posCam.x, posCam.y, posCam.z);
	DirectX::XMVECTOR posOlho = DirectX::XMLoadFloat3(&cSpotLight.Position);
	DirectX::XMVECTOR posOlhando = DirectX::XMLoadFloat4(&cam.PegarPosOlhando());
	DirectX::XMStoreFloat3(&cSpotLight.Direction, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(posOlhando, posOlho)));

	cPointLight.Position = DirectX::XMFLOAT3(posCam.x, posCam.y, posCam.z);
	
	dxd.SetarParametrosPorFrameShader3D(cDirLight, cPointLight, cSpotLight, DirectX::XMFLOAT3(posCam.x, posCam.y, posCam.z));

	if (cBostasComidas < cNBostas)
	{
		cFace.Atualizar();

		cFace.PosicionarCamera(&cam);
		cam.Atualizar();

		for (UINT i = 0; i < cBostas.size(); i++)
		{
			if (cBostas[i]->TestarColisaoFace(&cFace))
			{
				delete cBostas[i];
				cBostas[i] = NULL;

				cBostas[i] = cBostas.back();

				cBostas.pop_back();

				cBostasComidas++;
				cTinkle.Play();
			}
		}

		for (UINT i = 0; i < cBostas.size(); i++)
		{
			cBostas[i]->Atualizar();
		}
	}
	else
	{
		cFace.Rodar();

		static bool b = false;
		if (!b)
		{
			b = true;
			cFart.Play(-1000);
		}
	}

	cPiper.Atualizar();
}

void Jogo::Renderizar()
{	
	dxd.Ligar3D();
	
	cam.Renderizar(&cMapa);

	cam.Renderizar(&cFace);
	
	//cam.Renderizar(&cMapaBlocos);

	cam.Renderizar(&cPiper);

	for (UINT i = 0; i < cBostas.size(); i++)
	{
		cam.Renderizar(cBostas[i]);
	}

	dxd.Desligar3D();

	std::string s = "Grid map feature - "+std::to_string(cBostasComidas) + "/" + std::to_string(cNBostas);

	cFFixedSys.EscreverFrase(s, { dxd.PegarLargura() / 2.0f, 0.0f }, { 13.0f, 24.0f }, Fonte::CENTRO, &dxd);

	if (cNBostas <= cBostasComidas)
	{
		static float r = 0.0f;
		r += 0.01f;
		cImgGameOver.Renderizar(
		{ dxd.PegarLargura() / 2.0f + (sinf(15.0f * r) * 20.0f), dxd.PegarAltura() / 2.0f + (cosf(15.0f * r) * 20.0f) }, 
		{ 1.0f, 1.0f }, r, &dxd);
	}
}

void Jogo::MudouTamanhoJanela()
{
	dxd.QuandoRedimensionar();
}

void Jogo::CalcularEstatisticasDeTempo()
{
#if DEBUG || _DEBUG
	static int64_t frameCnt = 0;
	static float tempoPassado = 0.0f;

	frameCnt++;
	float tempoTotal = timer.TempoTotal();
	if ((tempoTotal - tempoPassado) >= 1.0)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wstring outs;

		outs = L"FPS: " + std::to_wstring(fps) + L" - Milsegundos: " + std::to_wstring(mspf) + L"\n";

		OutputDebugString(outs.c_str());

		frameCnt = 0;
		tempoPassado += 1.0f;
	}
#endif
}

void Jogo::Controles()
{
	if (cTeclado.BotaoEstaApertado(VK_UP))
	{
		cFace.ApertouCima();
	}
	if (cTeclado.BotaoEstaApertado(VK_DOWN))
	{
		cFace.ApertouBaixo();
	}
	if (cTeclado.BotaoEstaApertado(VK_RIGHT))
	{
		cFace.ApertouDireita();
	}
	if (cTeclado.BotaoEstaApertado(VK_LEFT))
	{
		cFace.ApertouEsquerda();
	}
	while (!cMouse.MouseEmpty())
	{
		MouseEvent me = cMouse.ReadMouse();
		switch (me.GetType())
		{
		case MouseEvent::WheelDown:
			{
				cFace.RodouBolinhaMouse(0.1f);
			}break;
		case MouseEvent::WheelUp:
			{
				cFace.RodouBolinhaMouse(-0.1f);
			}break;
		}
	}

	if (!cMousePrecionado && cMouse.LeftIsPressed())
	{
		cMousePrecionado = true;
		cPosMouseAnt = { (float)(cMouse.GetMouseX() / 50.0f), (float)(cMouse.GetMouseY() / 50.0f) };
	}
	else if(cMousePrecionado)
	{
		cMousePrecionado = false;

		DirectX::XMFLOAT2 cPosMouseAtual = { (float)(cMouse.GetMouseX() / 50.0f), (float)(cMouse.GetMouseY() / 50.0f) };

		cFace.MudouPosMouse({ cPosMouseAtual.x - cPosMouseAnt.x, cPosMouseAtual.y - cPosMouseAnt.y });
	}
}