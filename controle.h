#ifndef CONTROLE_H
#define CONTROLE_H
#include <stdbool.h>
#include <stdio.h>

//struct
typedef struct{
	char    numProcesso[27];
	char    tema[50];
	char    cpf[13];
	char    polo[20];
	int		codEstagiario;
	char    dtControle[12];
	char    proxTarefa[50];
	bool    temPrazoDobro;
	char    obsPrazo[50];
	char    audiencia[50];
	char    prazoSimples[12];
	char    prazoDobrado[12];
	char    movimentacaoProcessual[1000];
	int     diasProxControle;
	char    dtProposicao[12];
} Controle;


void atualizarOuInserirMovimentacao();
void adicionarMovimentacao(); //O arquivo CONTROLE.DAT � atualizado a partir da proposi��o da a��o num dos sistemas do Judici�rio ou quando h� movimenta��es processuais.
void verTodasMovimentacoes();
void gerarRelatorioControle(Controle* controle);
void adicionarDias(char* data, int dias_para_adicionar, char* data_resultado);
void atualizarMovimentacao(char* numProcesso);
void iniciarAtualizacaoMovimentacao();

#endif