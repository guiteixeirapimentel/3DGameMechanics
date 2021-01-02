#pragma once
#include <Windows.h>
#include <stdint.h>

class GameTimer
{
public:
	GameTimer();

	float TempoTotal() const;  // Em segundos
	float DeltaTime() const; // Em segundos

	void Reset();
	void Start();
	void Pausar();
	void Tick();

private:
	double cSegundosPorCount;
	double cDeltaTime;

	int64_t cTempoBase;
	int64_t cTempoPausado;
	int64_t cTempoPausa;
	int64_t cTempoAnterior;
	int64_t cTempoAtual;

	bool cPausado;
};