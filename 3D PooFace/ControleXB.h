#pragma once
#define WIN32_LEAN_AND_MEAN // We don't want the extra stuff like MFC and such
#include <Windows.h>
#include <Xinput.h>
#include <queue>

//#pragma comment(lib, "XINPUT9_1_0.lib")

#define XINPUTGETSTATE(nome) DWORD WINAPI nome(DWORD dwUserIndex, XINPUT_STATE* pState)
#define XINPUTSETSTATE(nome) DWORD WINAPI nome(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef XINPUTGETSTATE(XInputGetStateMeu);
typedef XINPUTSETSTATE(XInputSetStateMeu);

#define XInputGetState XInputGetState_ //Para nao haver conflitos e usar o meu XInputGetState
#define XInputSetState XInputSetState_ //Para nao haver conflitos e usar o meu XInputSetState

enum BOTAO
{
	Controle_A = 0,
	Controle_B = 1,
	Controle_X = 2,
	Controle_Y = 3,
	Controle_LB = 4,
	Controle_LT = 5,
	Controle_RT = 6,
	Controle_RB = 7,
	Controle_Back = 8,
	Controle_Start = 9,
	Controle_Setas_Cima = 10,
	Controle_Setas_Baixo = 11,
	Controle_Setas_Direita = 12,
	Controle_Setas_Esquerda = 13,
	Controle_Direcional_Esquerdo_Cima = 14,
	Controle_Direcional_Esquerdo_Baixo = 15,
	Controle_Direcional_Esquerdo_Direita = 16,
	Controle_Direcional_Esquerdo_Esquerda = 17,
	Controle_Direcional_Direito_Cima = 18,
	Controle_Direcional_Direito_Baixo = 19,
	Controle_Direcional_Direito_Direita = 20,
	Controle_Direcional_Direito_Esquerda = 21,
	NUM_TECLAS = 22,
	INVALIDO = 3540
};

struct Analogico
{
	float px;
	float py;
};

class EventoControle
{
public:
	enum TIPO
	{
		INVALIDO = 0,
		PRECIONADO = 1,
		SOLTO = 2
	};
public:
	EventoControle(TIPO tipo, BOTAO botao)
		:
		tipo(tipo),
		botao(botao)
	{}

	bool ÉValido() const
	{
		if (tipo != INVALIDO)
			return true;
		return false;
	}
	bool EstaPrecionado() const
	{
		if (tipo == PRECIONADO)
			return true;
		return false;
	}
	bool EstaSolto() const
	{
		if (tipo == SOLTO)
			return true;
		return false;
	}

	BOTAO PegarBotao() const
	{
		return botao;
	}
private:
	const TIPO tipo;
	const BOTAO botao;
};

class ControleXBCliente;

class ControleXBServer
{
	friend ControleXBCliente;
public:
	ControleXBServer(UINT id);
	void Checar();
private:
	void Vibrar(float normalVlEsq = 0.0f, float normalVlDir = 0.0f);
	void ChecarTeclas();
	void ChecarPosJG();
	Analogico PegaAnalogico(int n) const;

	bool CarregarXInput();
private:
	XINPUT_STATE estadoCtrl;
	DWORD ultimoValorPacote;
	bool teclas[NUM_TECLAS - 1];
	Analogico analogicoEsq;
	Analogico analogicoDir;
	const unsigned int id;

	XInputGetStateMeu *XInputGetState_;
	XInputSetStateMeu *XInputSetState_;

	std::queue<EventoControle> eventos;
};

class ControleXBCliente
{
public:
	ControleXBCliente(ControleXBServer &ctrlServer);
	bool BotaoEstaPrecionado(BOTAO botao) const;
	void Vibrar(float normalVlEsq = 0.0f, float normalVlDir = 0.0f) const;
	Analogico PegaAnalogico(int n) const;

	EventoControle PegarERetirarEventoControle();
	EventoControle PegarEventoControle();
private:
	ControleXBServer &ctrlServer;
};