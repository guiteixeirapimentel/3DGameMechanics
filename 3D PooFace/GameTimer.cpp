#include "GameTimer.h"

GameTimer::GameTimer()
	:
	cDeltaTime(-1.0),
	cPausado(false),
	cTempoAnterior(0),
	cTempoAtual(0),
	cTempoBase(0),
	cTempoPausa(0),
	cTempoPausado(0)
{
	int64_t countsPorSeg;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPorSeg);
	cSegundosPorCount = 1.0 / (double)countsPorSeg;

	Reset();
}

void GameTimer::Tick()
{
	if (cPausado)
	{
		cDeltaTime = 0.0;
		return;
	}
	//Pega Tempo ATUAL!
	int64_t tempoAtual;
	QueryPerformanceCounter((LARGE_INTEGER*)&tempoAtual);
	cTempoAtual = tempoAtual;

	cDeltaTime = (cTempoAtual - cTempoAnterior) * cSegundosPorCount;

	cTempoAnterior = cTempoAtual;

	if (cDeltaTime < 0.0)
	{
		cDeltaTime = 0.0;
	}
}

float GameTimer::DeltaTime() const
{
	return (float)cDeltaTime;
}

float GameTimer::TempoTotal() const
{
	if (cPausado)
	{
		return (float)(((cTempoPausa - cTempoPausado) - cTempoBase) * cSegundosPorCount);
	}
	else
	{
		return (float)(((cTempoAtual - cTempoPausado) - cTempoBase) * cSegundosPorCount);
	}
}


void GameTimer::Reset()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&cTempoAtual);

	cTempoBase = cTempoAtual;
	cTempoAnterior = cTempoAtual;
	cTempoPausa = 0;
	cPausado = false;
}

void GameTimer::Pausar()
{
	if (!cPausado)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&cTempoAtual);
		cPausado = true;
	}
}

void GameTimer::Start()
{
	int64_t startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (cPausado)
	{
		cTempoPausado += (startTime - cTempoPausa);

		cTempoAnterior = startTime;

		cTempoPausa = 0;
		cPausado = false;
	}
}