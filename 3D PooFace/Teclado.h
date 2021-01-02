#pragma once
#include <queue>

class TeclaEvento
{
public:
	enum EventoTipo
	{
		PRESSIONADO,
		SOLTO,
		INVALIDO
	};
private:
	EventoTipo tipo;
	unsigned char tecla;
public:
	TeclaEvento(EventoTipo tipo, unsigned char tecla)
		:
		tipo(tipo),
		tecla(tecla)
	{}

	bool EstaPrecionado() const
	{
		return tipo == PRESSIONADO;
	}
	bool EstaSolto() const
	{
		return tipo == SOLTO;
	}
	bool ÉValido() const
	{
		return tipo != INVALIDO;
	}

	unsigned char PegarTecla() const
	{
		return tecla;
	}
};

class TecladoServidor;

class TecladoCliente
{
public:
	TecladoCliente(TecladoServidor& teclado);
	bool BotaoEstaApertado(unsigned char c) const;
	unsigned char LerChar();
	unsigned char LerETirarChar();
	TeclaEvento LerTecla();
	TeclaEvento LerETirarTecla();
	bool TeclaVazia() const;
	bool CharVazio() const;
private:
	TecladoServidor& teclado;
};

class TecladoServidor
{
	friend TecladoCliente;
public:
	TecladoServidor();
	void BotaoFoiApertado(unsigned char c);
	void BotaoFoiSolto(unsigned char c);
	void AddFilaChars(unsigned char c);
	void LimparFilaChars();
private:
	static const int nBotoes = 256;
	bool botaoApertado[256];

	std::queue<unsigned char> filaDeChars;
	std::queue<TeclaEvento> filaDeTeclas;

	static const int TamanhoBuffer = 4;
};