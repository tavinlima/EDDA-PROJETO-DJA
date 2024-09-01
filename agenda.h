#ifndef AGENDA_H
#define AGENDA_H

#include <stdbool.h>
#include <glib.h>

typedef struct {
    int oab;
    int codEstagiario;
    char dtInscricao[11];
    char hrInscricao[6];
    char dtAgendada[11];
    char hrAgendada[6];
    char areaDireito[30];
    char tipoOrientacao[50];
    bool prazoFatal;
    char dtPrazoFatal[11];
    char observacoes[100];
    char numProcesso[27];
    char foiatend[100];
} Agenda;

void inicializarFila(int oab);
void enfileirar(Agenda novoAgendamento);
bool filaVazia(int oab);
Agenda* retirarDaFila(int oab);
void visualizarFila(int oab);
void visualizarTodasFilas();
void processarAtendimento();
void agendarAtendimento();
void gerarRelatorioAgenda(Agenda* agenda);
void adicionarObservacaoAtendimento();
Agenda* obterTodosAgendamentos(int *count);
void ordenaAgenda(Agenda* agendas, int tamanho_vetor);
void mostrarHistoricoAgendamentos();

#endif // AGENDA_H
