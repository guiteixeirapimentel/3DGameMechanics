#include "Teclado.h"

TecladoServidor::TecladoServidor()
{}
void TecladoServidor::BotaoFoiApertado(unsigned char c)
{
	filaDeTeclas.push(TeclaEvento(TeclaEvento::PRESSIONADO, c));
	botaoApertado[c] = true;
	if (filaDeTeclas.size() > TamanhoBuffer)
	{
		filaDeTeclas.pop();
	}
}

void TecladoServidor::BotaoFoiSolto(unsigned char c)
{
	botaoApertado[c] = false;
	filaDeTeclas.push(TeclaEvento(TeclaEvento::SOLTO, c));
	if (filaDeTeclas.size() > TamanhoBuffer)
	{
		filaDeTeclas.pop();
	}
}

void TecladoServidor::AddFilaChars(unsigned char c)
{
	filaDeChars.push(c);
}

void TecladoServidor::LimparFilaChars()
{
	filaDeChars.empty();
}



TecladoCliente::TecladoCliente(TecladoServidor& teclado)
	:
	teclado(teclado)
{}

bool TecladoCliente::BotaoEstaApertado(unsigned char c) const
{
	return teclado.botaoApertado[c];
}

bool TecladoCliente::TeclaVazia() const
{
	return teclado.filaDeTeclas.empty();
}

unsigned char TecladoCliente::LerChar()
{
	if (teclado.filaDeChars.size() > 0)
	{
		return teclado.filaDeChars.front();
	}
	return 0;
}

bool TecladoCliente::CharVazio() const
{
	return teclado.filaDeChars.empty();
}

TeclaEvento TecladoCliente::LerTecla()
{
	if (teclado.filaDeTeclas.size() > 0)
	{
		return teclado.filaDeTeclas.front();
	}
	return TeclaEvento(TeclaEvento::INVALIDO, 0);
}

unsigned char TecladoCliente::LerETirarChar()
{
	if (teclado.filaDeChars.size() > 0)
	{
		unsigned char c = teclado.filaDeChars.front();
		teclado.filaDeChars.pop();
		return c;
	}
	return 0;
}

TeclaEvento TecladoCliente::LerETirarTecla()
{
	if (teclado.filaDeTeclas.size() > 0)
	{
		TeclaEvento c = teclado.filaDeTeclas.front();
		teclado.filaDeTeclas.pop();
		return c;
	}
	return TeclaEvento(TeclaEvento::INVALIDO, 0);
}