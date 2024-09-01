#define _CRT_SECURE_NO_WARNINGS // desabilita alertas de unsafe

// Arquivos
#include "controle.h"
#include "global.h"
#include "estagiario.h"

// bilbiotecas
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

// Variáveis globais
FILE *arquivoControle;
Controle movimentacao;
Controle *movimentacoes;

void atualizarOuInserirMovimentacao()
{
	int opcao = 1;

	printf("\n----------- INSERÇÃO DE MOVIMENTAÇÃO NO SISTEMA -------------\n");
	printf("\n1 - Inserir movimentação\n");
	printf("2 - Atualizar movimentação\n");
	scanf("%i", &opcao);
	limparBuffer();

	switch (opcao)
	{
	case 1:
		adicionarMovimentacao();
		break;
	case 2:
		iniciarAtualizacaoMovimentacao();
		break;
	default:
		break;
	}
}

void adicionarMovimentacao()
{
	// O arquivo CONTROLE.DAT é atualizado a partir da proposição da ação num dos sistemas do Judiciário ou quando há movimentações processuais.

	printf("\n----------- INSERÇÃO DE MOVIMENTAÇÃO NO SISTEMA -------------\n");

	printf("\nInsira o número do processo: ");
	fgets(movimentacao.numProcesso, sizeof(movimentacao.numProcesso), stdin);
	movimentacao.numProcesso[strcspn(movimentacao.numProcesso, "\n")] = '\0';
	limparBuffer();

	printf("\nInsira o tema do processo: ");
	fgets(movimentacao.tema, sizeof(movimentacao.tema), stdin);
	movimentacao.tema[strcspn(movimentacao.tema, "\n")] = '\0';

	printf("\nInsira o cpf do cliente: ");
	fgets(movimentacao.cpf, sizeof(movimentacao.cpf), stdin);
	movimentacao.cpf[strcspn(movimentacao.cpf, "\n")] = '\0';

	printf("\nPolo (se é reclamante/reclamado, exequente/executado): ");
	fgets(movimentacao.polo, sizeof(movimentacao.polo), stdin);
	movimentacao.polo[strcspn(movimentacao.polo, "\n")] = '\0';

	Estagiario estag;

	do
	{
		printf("\nInsira o código do estagiário responsável: ");
		scanf("%i", &movimentacao.codEstagiario);
		limparBuffer();
		estag = retornarEstagiarioEspecifico(movimentacao.codEstagiario);
		if (estag.codeEstag == -1)
			printf("\nEsse estagiário não está cadastrado no sistema.\n");

		else if (estag.tipo != 'P')
		{
			printf("\nEstagiário inválido.\n");
		}
	} while (estag.tipo != 'P' || estag.codeEstag == -1);

	printf("\nQual a próxima atividade a ser realizada: ");
	fgets(movimentacao.proxTarefa, sizeof(movimentacao.proxTarefa), stdin);
	movimentacao.proxTarefa[strcspn(movimentacao.proxTarefa, "\n")] = '\0';

	printf("\n Foi deferido prazo em dobro para o DJe? (S/N): ");
	if (getchar() == 'S')
		movimentacao.temPrazoDobro = true;
	else
		movimentacao.temPrazoDobro = false;
	limparBuffer();

	printf("\nAlguma observação relacionada ao prazo a cumprir: ");
	fgets(movimentacao.obsPrazo, sizeof(movimentacao.obsPrazo), stdin);
	movimentacao.obsPrazo[strcspn(movimentacao.obsPrazo, "\n")] = '\0';

	printf("\nDados relacionados à audiência (Se está marcada, etc): ");
	fgets(movimentacao.audiencia, sizeof(movimentacao.audiencia), stdin);
	movimentacao.audiencia[strcspn(movimentacao.audiencia, "\n")] = '\0';

	printf("\nData do prazo simples: ");
	fgets(movimentacao.prazoSimples, sizeof(movimentacao.prazoSimples), stdin);
	movimentacao.prazoSimples[strcspn(movimentacao.prazoSimples, "\n")] = '\0';

	printf("\nData do prazo em dobro: ");
	fgets(movimentacao.prazoDobrado, sizeof(movimentacao.prazoDobrado), stdin);
	movimentacao.prazoDobrado[strcspn(movimentacao.prazoDobrado, "\n")] = '\0';

	printf("\nRegistro da movimentação processual: ");
	fgets(movimentacao.movimentacaoProcessual, sizeof(movimentacao.movimentacaoProcessual), stdin);
	movimentacao.movimentacaoProcessual[strcspn(movimentacao.movimentacaoProcessual, "\n")] = '\0';

	printf("\nQuantos dias até o próximo controle: ");
	scanf("%i", &movimentacao.diasProxControle);
	limparBuffer();

	printf("\nData em que a ação foi protocolada num dos sistemas judiciários: ");
	fgets(movimentacao.dtProposicao, sizeof(movimentacao.dtProposicao), stdin);
	movimentacao.dtProposicao[strcspn(movimentacao.dtProposicao, "\n")] = '\0';

	adicionarDias(movimentacao.dtProposicao, movimentacao.diasProxControle, movimentacao.dtControle);

	arquivoControle = fopen("./data/CONTROLE.DAT", "a+b");

	if (arquivoControle == NULL)
	{
		printf("Erro ao abrir CONTROLE.DAT\n");
		system("pause");
		exit(0);
	};

	fwrite(&movimentacao, sizeof(Controle), 1, arquivoControle);
	fclose(arquivoControle);

	printf("\nCONTROLE ADICIONADO NO SISTEMA COM SUCESSO!\n");
	printf("--------------------------------------------------------\n");
}

void verTodasMovimentacoes()
{
	int count = 0;
	arquivoControle = fopen("./data/CONTROLE.DAT", "r+b");

	if (arquivoControle == NULL)
	{
		printf("Erro ao abrir CONTROLE.DAT\n");
		system("pause");
		exit(0);
	};

	printf("\n----------- LISTA DE CASOS NO SISTEMA -------------\n");

	while (!feof(arquivoControle))
	{
		fread(&movimentacao, sizeof(movimentacao), 1, arquivoControle);
		if (!feof(arquivoControle))
		{
			count++;
			gerarRelatorioControle(&movimentacao);
		}
		if (count == 0)
			printf("\nNenhum caso em andamento\n");
	}
	count = 0;

	fclose(arquivoControle);

	printf("\n-------------------------------------------------------\n");

	system("pause");
}

void gerarRelatorioControle(Controle *controle)
{
	printf("\nNúmero do Processo:		%s\n", controle->numProcesso);
	printf("Tema:				%s\n", controle->tema);
	printf("CPF:				%s\n", controle->cpf);
	printf("Polo:				%s\n", controle->polo);
	printf("Código do estagiário:		%i\n", controle->codEstagiario);
	printf("Data Controle:			%s\n", controle->dtControle);
	printf("Próxima tarefa:			%s\n", controle->proxTarefa);
	printf("Tem prazo em dobro:		%s\n", controle->temPrazoDobro ? "Verdadeiro" : "Falso");
	printf("Observação do prazo:		%s\n", controle->obsPrazo);
	printf("Audiência:			%s\n", controle->audiencia);
	printf("Prazo simples:			%s\n", controle->prazoSimples);
	printf("Prazo dobrado:			%s\n", controle->prazoDobrado);
	printf("Movimentacao Processual:	%s\n", controle->movimentacaoProcessual);
	printf("Dias proximo controle:		%i\n", controle->diasProxControle);
	printf("Data proposição:		%s\n", controle->dtProposicao);
}

void adicionarDias(char *data, int dias_para_adicionar, char *data_resultado)
{
	struct tm tm_data = {0};
	sscanf(data, "%d/%d/%d", &tm_data.tm_mday, &tm_data.tm_mon, &tm_data.tm_year);
	tm_data.tm_mon -= 1;
	tm_data.tm_year -= 1900;

	tm_data.tm_mday += dias_para_adicionar;

	mktime(&tm_data);

	strftime(data_resultado, 12, "%d/%m/%Y", &tm_data);
}

void atualizarMovimentacao(char *numProcesso)
{
	bool encontrado = false;
	Controle controleAtualizado;

	FILE *arquivoControle = fopen("./data/CONTROLE.DAT", "r+b");
	if (arquivoControle == NULL)
	{
		printf("Erro ao abrir CONTROLE.DAT\n");
		system("pause");
		exit(0);
	}

	while (fread(&movimentacao, sizeof(movimentacao), 1, arquivoControle))
	{

		if (strcmp(movimentacao.numProcesso, numProcesso) == 0)
		{
			encontrado = true;
			printf("\n----------- ATUALIZAÇÃO DE PROCESSO -------------\n");
			printf("\nDados atuais do processo:\n");
			gerarRelatorioControle(&movimentacao);

			printf("\nInsira os novos dados do processo (ou deixe em branco para manter o atual):\n");

			int tempCodEstagiario = 0;
			printf("\nNovo código de estagiário (atual: %i): ", movimentacao.codEstagiario);

			scanf("%i", &tempCodEstagiario);
			limparBuffer();

			if (&tempCodEstagiario != NULL)
			{
				movimentacao.codEstagiario = tempCodEstagiario;
			}
			limparBuffer();

			printf("\nNova Data Controle (atual: %s): ", movimentacao.dtControle);
			fgets(controleAtualizado.dtControle, sizeof(controleAtualizado.dtControle), stdin);
			if (strcmp(controleAtualizado.dtControle, "\n") != 0)
			{
				controleAtualizado.dtControle[strcspn(controleAtualizado.dtControle, "\n")] = '\0';
				strcpy(movimentacao.dtControle, controleAtualizado.dtControle);
			}

			printf("\nNova Próxima Tarefa (atual: %s): ", movimentacao.proxTarefa);
			fgets(controleAtualizado.proxTarefa, sizeof(controleAtualizado.proxTarefa), stdin);
			if (strcmp(controleAtualizado.proxTarefa, "\n") != 0)
			{
				controleAtualizado.proxTarefa[strcspn(controleAtualizado.proxTarefa, "\n")] = '\0';
				strcpy(movimentacao.proxTarefa, controleAtualizado.proxTarefa);
			}

			printf("\n Foi deferido prazo em dobro para o DJe? (S/N): ");
			if (getchar() == 'S')
				movimentacao.temPrazoDobro = true;
			else
				movimentacao.temPrazoDobro = false;
			limparBuffer();

			printf("\nNova Observação do Prazo (atual: %s): ", movimentacao.obsPrazo);
			fgets(controleAtualizado.obsPrazo, sizeof(controleAtualizado.obsPrazo), stdin);
			if (strcmp(controleAtualizado.obsPrazo, "\n") != 0)
			{
				controleAtualizado.obsPrazo[strcspn(controleAtualizado.obsPrazo, "\n")] = '\0';
				strcpy(movimentacao.obsPrazo, controleAtualizado.obsPrazo);
			}

			printf("\nNovos dados sobre a audiência (atual: %s): ", movimentacao.audiencia);
			fgets(controleAtualizado.audiencia, sizeof(controleAtualizado.audiencia), stdin);
			if (strcmp(controleAtualizado.audiencia, "\n") != 0)
			{
				controleAtualizado.audiencia[strcspn(controleAtualizado.audiencia, "\n")] = '\0';
				strcpy(movimentacao.audiencia, controleAtualizado.audiencia);
			}

			printf("\nNovo Prazo Simples (atual: %s): ", movimentacao.prazoSimples);
			fgets(controleAtualizado.prazoSimples, sizeof(controleAtualizado.prazoSimples), stdin);
			if (strcmp(controleAtualizado.prazoSimples, "\n") != 0)
			{
				controleAtualizado.prazoSimples[strcspn(controleAtualizado.prazoSimples, "\n")] = '\0';
				strcpy(movimentacao.prazoSimples, controleAtualizado.prazoSimples);
			}

			printf("\nNovo Prazo Dobrado (atual: %s): ", movimentacao.prazoDobrado);
			fgets(controleAtualizado.prazoDobrado, sizeof(controleAtualizado.prazoDobrado), stdin);
			if (strcmp(controleAtualizado.prazoDobrado, "\n") != 0)
			{
				controleAtualizado.prazoDobrado[strcspn(controleAtualizado.prazoDobrado, "\n")] = '\0';
				strcpy(movimentacao.prazoDobrado, controleAtualizado.prazoDobrado);
			}

			printf("\nNovos Dados sobre a movimentação processual (atual: %s): ", movimentacao.movimentacaoProcessual);
			fgets(controleAtualizado.movimentacaoProcessual, sizeof(controleAtualizado.movimentacaoProcessual), stdin);
			if (strcmp(controleAtualizado.movimentacaoProcessual, "\n") != 0)
			{
				controleAtualizado.movimentacaoProcessual[strcspn(controleAtualizado.movimentacaoProcessual, "\n")] = '\0';
				strcpy(movimentacao.movimentacaoProcessual, controleAtualizado.movimentacaoProcessual);
			}

			int tempDiasProxControle;
			printf("\nDias próximo controle (atual: %i): ", movimentacao.diasProxControle);
			scanf("%i", &tempDiasProxControle);
			limparBuffer();
			if (tempDiasProxControle > 0 && &tempDiasProxControle != NULL)
			{
				movimentacao.diasProxControle = tempDiasProxControle;
			}

			printf("\nNova data em que a ação foi protocolada num dos sistemas judiciários(atual: %s): ", movimentacao.dtProposicao);
			fgets(controleAtualizado.dtProposicao, sizeof(controleAtualizado.dtProposicao), stdin);
			if (strcmp(controleAtualizado.dtProposicao, "\n") != 0)
			{
				controleAtualizado.dtProposicao[strcspn(controleAtualizado.dtProposicao, "\n")] = '\0';
				strcpy(movimentacao.dtProposicao, controleAtualizado.dtProposicao);
			}

			fseek(arquivoControle, sizeof(Controle), SEEK_CUR);

			fwrite(&movimentacao, sizeof(Controle), 1, arquivoControle);
			printf("\nPROCESSO ATUALIZADO COM SUCESSO!\n");
			break;
		}
	}

	if (!encontrado)
	{
		printf("\nNenhum processo encontrado com o número de processo: %s\n", numProcesso);
	}

	fclose(arquivoControle);
}

void iniciarAtualizacaoMovimentacao()
{
	verTodasMovimentacoes();

	char numProcesso[27];

	limparBuffer();
	printf("\nQual o número do processo que você deseja atualizar? ");
	fgets(numProcesso, sizeof(numProcesso), stdin);
	numProcesso[strcspn(numProcesso, "\n")] = '\0';

	atualizarMovimentacao(numProcesso);
}