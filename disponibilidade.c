#define _CRT_SECURE_NO_WARNINGS //desabilita alertas de unsafe

//Arquivos
#include "disponibilidade.h"
#include "global.h"
#include "advogado.h"

//Bilbiotecas
#include <stdio.h> //biblioteca padrão de entrada e saída
#include <string.h> //biblioteca para manipulação de strings
#include <stdbool.h> //biblioteca para usar tipo de dados booleano
#include <stdlib.h> //biblioteca para usar funções de alocação de memória dinâmica

//Variáveis globais
FILE* arquivoDisponibilidade;
Disponibilidade disponibilidade;
Disponibilidade* disponibilidades;
Advogado adv;



void adicionarDisponibilidade() {
	//Variáveis temporárias
	int tempOAB;
	int tempDiaSemana;
	char tempFormato;

	printf("\n----------- INSERÇÃO DE DISPONIBILIDADE NO SISTEMA -------------\n");

	// Enquanto não encontrar um advogado com a OAB informada, pede para inserir novamente
	do {
		printf("Qual o número da OAB do advogado: ");
		scanf("%i", &tempOAB);
		limparBuffer();
		adv = retornarAdvogadoEspecifico(tempOAB);
	} while (adv.oab == -1); //  o código -1 é quando não encontra o advogado
	// Atribui o número da OAB à disponibilidade
	disponibilidade.oab = tempOAB;

	// Validação do dia da semana, enquanto não for entre 1 e 5, pede para inserir novamente
	do {
		printf("\nQual o dia da semana: ");
		scanf("%d", &tempDiaSemana);
		limparBuffer();
	} while (tempDiaSemana < 1 || tempDiaSemana > 5);
	// Atribui o dia da semana à disponibilidade
	disponibilidade.diaSemana = tempDiaSemana;

	printf("\nQual o horário de início: ");
	fgets(disponibilidade.horaInicio, sizeof(disponibilidade.horaInicio), stdin);
	disponibilidade.horaInicio[strcspn(disponibilidade.horaInicio, "\n")] = '\0';
	printf("\nQual o horário de término: ");
	fgets(disponibilidade.horaFim, sizeof(disponibilidade.horaFim), stdin);
	disponibilidade.horaFim[strcspn(disponibilidade.horaFim, "\n")] = '\0';
	// Validação do formato, enquanto nao for R ou P, pede para inserir novamente
	do {
		printf("\nQual o formato da disponibilidade (R - Remoto / P - Presencial): ");
		tempFormato = getchar();
		limparBuffer();

	} while (tempFormato != 'R' && tempFormato != 'P');
	// Atribui o formato à disponibilidade
	disponibilidade.formato = tempFormato;

	// Abre, verifica se o arquivo foi aberto, escreve e fecha o arquivo
	arquivoDisponibilidade = fopen("./data/DISPONIBILIDADE.DAT", "a+b");
	if (arquivoDisponibilidade == NULL)
	{
		printf("Erro ao abrir DISPONIBILIDADE.DAT\n");
		system("pause");
		exit(0);
	}
	fwrite(&disponibilidade, sizeof(Disponibilidade), 1, arquivoDisponibilidade);
	fclose(arquivoDisponibilidade);

	printf("\nREGISTRO DE DISPONIBILIDADE ADICIONADO NO SISTEMA!\n");
	//Imprime o relatório da disponibilidade adicionada
	gerarRelatorioDisponibilidade(&disponibilidade);
	printf("\n--------------------------------------------------------\n");

}


//Mostra o relatório de todas as disponibilidades mesclado com as informações do advogado
void mostrarRelatorioAdvDis() {
	//Variáveis temporárias
	int diaSemana;

	//chama a função que armazena todas as disponibilidades em um array de Disponibilidade e retorna a quantidade de disponibilidades
	int count = obterTodasDisponibilidades();

	// Obter o dia da semana (hoje)
	diaSemana = retornarDiaSemana();

	if(diaSemana >= 5) { //0 - domingo, 1 - segunda, 2 - terça, 3 - quarta, 4 - quinta, 5 - sexta, 6 - sábado
		diaSemana = 0;
	}


	//percorre o array de disponibilidades e verifica se há disponibilidade do dia da semana informado + 1
	//Exemplo: se hoje é segunda-feira, vai ser retornado as disponibilidades de terça-feira
	for (int i = 0; i < count; i++) {
		if (disponibilidades[i].diaSemana == diaSemana + 1) {
			adv = retornarAdvogadoEspecifico(disponibilidades[i].oab);
			gerarRelatorioAdvDis(&disponibilidades[i], adv);
			printf("\n          ¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨         ");
		}
	}
}


//Retorna a disponibilidade específica do advogado com a OAB e do dia da semana informados.
Disponibilidade obterDisponibilidadeEspecifica(int oab, int diaSemana)
{
	//chama a função que armazena todas as disponibilidades em um array de Disponibilidade e retorna a quantidade de disponibilidades
	int count = obterTodasDisponibilidades();

	if(diaSemana >= 5) {
		diaSemana = 1;
	}
	//percorre o array de disponibilidades e verifica se há disponibilidade do advogado com a OAB e do dia da semana informados
	for (int i = 0; i < count; i++)
	{
		if (disponibilidades[i].oab == oab && disponibilidades[i].diaSemana == diaSemana)
		{
			return disponibilidades[i];
		}
	}

	//Se não encontrar a disponibilidade, exibe a mensagem de erro
	printf("Nenhuma disponibilidade encontrada para o advogado com OAB: %d e dia da semana: %d\n\n", oab, diaSemana);

	//Retorna uma disponibilidade inválida para validação em outras funções
	Disponibilidade disp_invalida;
	disp_invalida.oab = -1;
	return disp_invalida;
}

//Armazena todas as disponibilidades em um array de Disponibilidade
int obterTodasDisponibilidades() {
	int count = 0;

	// Abre o arquivo, verifica se foi aberto
	arquivoDisponibilidade = fopen("./data/DISPONIBILIDADE.DAT", "rb");

	if (arquivoDisponibilidade == NULL)
	{
		printf("Erro ao abrir DISPONIBILIDADE.DAT\n");
		system("pause");
		exit(0);
	};

	// Vai até o final do arquivo
	fseek(arquivoDisponibilidade, 0, SEEK_END);
	// Obtém a posição atual do cursor (tamanho do arquivo em bytes)
	long tamanhoArquivo = ftell(arquivoDisponibilidade);
	// Retorna o cursor para o início do arquivo
	rewind(arquivoDisponibilidade);

	//Alocação de memória para o array de disponibilidades
	disponibilidades = (Disponibilidade*)malloc(tamanhoArquivo);

	//Verifica se a alocação de memória foi bem sucedida
	if (disponibilidades == NULL) {
		printf("Erro interno. (FALHA NA ALOCAÇÃO DE MEMÓRIA)\n");
		system("pause");
		exit(0);
	}

	//Lê as disponibilidades do arquivo e armazena no array de disponibilidades
	while (!feof(arquivoDisponibilidade)) {
		fread(&disponibilidade, sizeof(disponibilidade), 1, arquivoDisponibilidade);
		if (!feof(arquivoDisponibilidade)) {
			disponibilidades[count] = disponibilidade;
			count++;
		}
	}

	//Fecha o arquivo
	fclose(arquivoDisponibilidade);

	//Retorna a quantidade de disponibilidades 
	return count;
}

//Uma mescla de informações do advogado e da disponibilidade, conforme solicitado no trabalho
void gerarRelatorioAdvDis(Disponibilidade* disponibilidade, Advogado adv) {
	printf("\nOAB:                           %i", adv.oab);
	printf("\nAdvogado e disponibilidade:    %s (%s)", adv.nome, adv.especialidade);
	printf("\nEmail:                         %s", adv.email);
	switch (disponibilidade->diaSemana)
	{
	case 1:
		printf("\nDia da semana:                 Segunda-feira");
		break;
	case 2:
		printf("\nDia da semana:                 Terça-feira");
		break;
	case 3:
		printf("\nDia da semana:                 Quarta-feira");
		break;
	case 4:
		printf("\nDia da semana:                 Quinta-feira");
		break;
	case 5:
		printf("\nDia da semana:                 Sexta-feira");
		break;
	default:
		break;
	}
	printf("\nHorário de início:             %s", disponibilidade->horaInicio);
	printf("\nHorário de término:            %s", disponibilidade->horaFim);
	printf("\nFormato:                       %s\n", disponibilidade->formato == 'R' ? "Remoto" : "Presencial");
}


//Relatório de disponibilidade 
void gerarRelatorioDisponibilidade(Disponibilidade* disponibilidade) {
	printf("\nNúmero da OAB: %i", disponibilidade->oab);

	switch (disponibilidade->diaSemana)
	{
	case 1:
		printf("\nDia da semana: Segunda-feira");
		break;
	case 2:
		printf("\nDia da semana: Terça-feira");
		break;
	case 3:
		printf("\nDia da semana: Quarta-feira");
		break;
	case 4:
		printf("\nDia da semana: Quinta-feira");
		break;
	case 5:
		printf("\nDia da semana: Sexta-feira");
		break;
	default:
		break;
	}
	printf("\nHorário de início: %s", disponibilidade->horaInicio);
	printf("\nHorário de término: %s", disponibilidade->horaFim);
	printf("\nFormato: %s", disponibilidade->formato == 'R' ? "Remoto" : "Presencial");
}