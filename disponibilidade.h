#ifndef DISPONIBILIDADE_H
#define DISPONIBILIDADE_H

#include "advogado.h"

typedef struct {
	int oab;
	int diaSemana;
	char horaInicio[7];
	char horaFim[7];
	char formato;
} Disponibilidade;


void adicionarDisponibilidade();
Disponibilidade obterDisponibilidadeEspecifica(int oab, int diaSemana);
int obterTodasDisponibilidades();
void gerarRelatorioDisponibilidade(Disponibilidade* disponibilidade);
void gerarRelatorioAdvDis(Disponibilidade* disponibilidade, Advogado adv);
void mostrarRelatorioAdvDis();

#endif

