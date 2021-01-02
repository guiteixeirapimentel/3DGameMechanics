#include "ControleXB.h"

XINPUTGETSTATE(XInputGetStateVazio)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}
XINPUTSETSTATE(XInputSetStateVazio)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}

ControleXBServer::ControleXBServer(UINT id)
	:
	id(id),
	XInputGetState_(XInputGetStateVazio),
	XInputSetState_(XInputSetStateVazio)
{
	memset(&teclas, false, NUM_TECLAS - 1);

	CarregarXInput();
}

void ControleXBServer::Checar()
{
	memset(&estadoCtrl, 0, sizeof(XINPUT_STATE));
	DWORD resultado = XInputGetState(id, &estadoCtrl);

	if (resultado != ERROR_SUCCESS)
	{
		//	MessageBox(NULL, L"Erro ao conectar ao controle!", L"Não foi possivel conectar ao controle!", 1);
		return;
	}
	if (estadoCtrl.dwPacketNumber != ultimoValorPacote)//Se o pacote atual é igual entao nada aconteceu
	{
		ultimoValorPacote = estadoCtrl.dwPacketNumber;

		ChecarTeclas();
		ChecarPosJG();
	}
}

//Checa Teclas(A,B,Y,X,Direcional)
void ControleXBServer::ChecarTeclas()
{
	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if (!teclas[Controle_A])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_A));
			teclas[Controle_A] = true;
		}
	}
	else
	{
		if (teclas[Controle_A])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_A));
			teclas[Controle_A] = false;
		}
	}

	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		if (!teclas[!Controle_B])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_B));
			teclas[Controle_B] = true;
		}
	}
	else
	{
		if (teclas[Controle_B])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_B));
			teclas[Controle_B] = false;
		}
	}
	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		if (!teclas[Controle_X])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_X));
			teclas[Controle_X] = true;
		}
	}
	else
	{
		if (teclas[Controle_X])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_X));
			teclas[Controle_X] = false;
		}
	}
	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		if (!teclas[Controle_Y])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_Y));
			teclas[Controle_Y] = true;
		}
	}
	else
	{
		if (teclas[Controle_Y])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_Y));
			teclas[Controle_Y] = false;
		}
	}
	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		if (!teclas[Controle_Setas_Cima])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_Setas_Cima));
			teclas[Controle_Setas_Cima] = true;
		}
	}
	else
	{
		if (teclas[Controle_Setas_Cima])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_Setas_Cima));
			teclas[Controle_Setas_Cima] = false;
		}
	}
	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		if (!teclas[Controle_Setas_Baixo])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_Setas_Baixo));
			teclas[Controle_Setas_Baixo] = true;
		}
	}
	else
	{
		if (teclas[Controle_Setas_Baixo])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_Setas_Baixo));
			teclas[Controle_Setas_Baixo] = false;
		}
	}
	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if (!teclas[Controle_Setas_Direita])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_Setas_Direita));
			teclas[Controle_Setas_Direita] = true;
		}
	}
	else
	{
		if (teclas[Controle_Setas_Direita])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_Setas_Direita));
			teclas[Controle_Setas_Direita] = false;
		}
	}
	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		if (!teclas[Controle_Setas_Esquerda])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_Setas_Esquerda));
			teclas[Controle_Setas_Esquerda] = true;
		}
	}
	else
	{
		if (teclas[Controle_Setas_Esquerda])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_Setas_Esquerda));
			teclas[Controle_Setas_Esquerda] = false;
		}
	}
	if (estadoCtrl.Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		if (!teclas[Controle_Start])
		{
			eventos.push(EventoControle(EventoControle::PRECIONADO, Controle_Start));
			teclas[Controle_Start] = true;
		}
	}
	else
	{
		if (teclas[Controle_Start])
		{
			eventos.push(EventoControle(EventoControle::SOLTO, Controle_Start));
			teclas[Controle_Start] = false;
		}
	}
}

//Checa gatilhos e analogico
void ControleXBServer::ChecarPosJG()
{
	if (estadoCtrl.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		teclas[Controle_RB] = true;
	}
	else
	{
		teclas[Controle_RB] = false;
	}

	if (estadoCtrl.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		teclas[Controle_RT] = true;
	}
	else
	{
		teclas[Controle_RT] = false;
	}

	if ((estadoCtrl.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		estadoCtrl.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(estadoCtrl.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			estadoCtrl.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		estadoCtrl.Gamepad.sThumbLX = 0;
		estadoCtrl.Gamepad.sThumbLY = 0;
	}

	// Check left thumbStick

	float LX = estadoCtrl.Gamepad.sThumbLX;
	float LY = estadoCtrl.Gamepad.sThumbLY;
	if (LX != 0 || LY != 0)
	{
		//determine how far the controller is pushed
		//float magnitude = sqrt(LX*LX + LY*LY);

		//determine the direction the controller is pushed
		float normalizedLX = LX / (32727 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);  // magnitude;
		float normalizedLY = LY / (32727 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		analogicoEsq = { normalizedLX, normalizedLY };
	}
	else
	{
		analogicoEsq = { 0, 0 };
	}
}

void ControleXBServer::Vibrar(float vlEsq, float vlDir)
{
	XINPUT_VIBRATION vibracao;
	vibracao.wLeftMotorSpeed = (WORD)(vlEsq * 65535);
	vibracao.wRightMotorSpeed = (WORD)(vlDir * 65535);
	XInputSetState(id, &vibracao);
}

Analogico ControleXBServer::PegaAnalogico(int n) const
{
	switch (n)
	{
	case 0:
		return analogicoEsq;
		break;
	case 1:
		return analogicoDir;
		break;
	default:
		return analogicoEsq;
		break;
	}
}

bool ControleXBServer::CarregarXInput()
{
	HMODULE XInputLib = LoadLibrary(L"xinput1_4.dll");
	if (!XInputLib)
	{
		XInputLib = LoadLibrary(L"xinput1_3.dll");
	}

	if (XInputLib)
	{
		XInputGetState = (XInputGetStateMeu*)GetProcAddress(XInputLib, "XInputGetState");
		XInputSetState = (XInputSetStateMeu*)GetProcAddress(XInputLib, "XInputSetState");
		return true;
	}
	return false;
}

ControleXBCliente::ControleXBCliente(ControleXBServer& ctrlServer)
	:
	ctrlServer(ctrlServer)
{}

bool ControleXBCliente::BotaoEstaPrecionado(BOTAO botao) const
{
	return ctrlServer.teclas[botao];
}
void ControleXBCliente::Vibrar(float normalVlEsq, float normalVlDir) const
{
	ctrlServer.Vibrar(normalVlEsq, normalVlDir);
}
Analogico ControleXBCliente::PegaAnalogico(int n) const
{
	return ctrlServer.PegaAnalogico(n);
}

EventoControle ControleXBCliente::PegarEventoControle()
{
	if (ctrlServer.eventos.size() > 0)
	{
		EventoControle c = ctrlServer.eventos.front();
		return c;
	}
	return EventoControle(EventoControle::INVALIDO, INVALIDO);
}

EventoControle ControleXBCliente::PegarERetirarEventoControle()
{
	if (ctrlServer.eventos.size() > 0)
	{
		EventoControle c = ctrlServer.eventos.front();
		ctrlServer.eventos.pop();
		return c;
	}
	return EventoControle(EventoControle::INVALIDO, INVALIDO);
}