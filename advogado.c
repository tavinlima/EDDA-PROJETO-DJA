#define _CRT_SECURE_NO_WARNINGS //desabilita alertas de unsafe


//Módulos
#include "advogado.h"
#include "global.h"

//bilbiotecas
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>

//Variáveis globais
FILE* arquivoAdvogado;
Advogado advogado;
Advogado* advogados;



void adicionarAdvogado()
{
	printf("\n----------- INSERÇÃO DE ADVOGADO NO SISTEMA -------------\n");

	printf("\nInsira o nome do advogado: ");
	fgets(advogado.nome, sizeof(advogado.nome), stdin);
	advogado.nome[strcspn(advogado.nome, "\n")] = '\0';

	printf("\nInsira o número da OAB do advogado: ");
	scanf("%i", &advogado.oab);
	limparBuffer();

	printf("\nInsira o CPF do advogado: ");
	fgets(advogado.cpf, sizeof(advogado.cpf), stdin);
	advogado.cpf[strcspn(advogado.cpf, "\n")] = '\0';

	printf("\nInsira o número de telefone do advogado: ");
	fgets(advogado.numeroTelefone, sizeof(advogado.numeroTelefone), stdin);
	advogado.numeroTelefone[strcspn(advogado.numeroTelefone, "\n")] = '\0';

	printf("\nInsira o email do advogado: ");
	fgets(advogado.email, sizeof(advogado.email), stdin);
	advogado.email[strcspn(advogado.email, "\n")] = '\0';

	printf("\nInsira a especialidade do advogado: ");
	fgets(advogado.especialidade, sizeof(advogado.especialidade), stdin);
	advogado.especialidade[strcspn(advogado.especialidade, "\n")] = '\0';

	int tempAnoIngresso;

	do {
		printf("\nInsira o ano de ingresso do advogado: ");
		scanf("%i", &tempAnoIngresso);
		limparBuffer();

		if (tempAnoIngresso < 1970 || tempAnoIngresso > 2024)
			printf("\nAno de ingresso inválido. Insira um ano entre 1970 e %i.\n", 2024);

	} while (tempAnoIngresso < 1970 || tempAnoIngresso > 2024);

	advogado.anoIngresso = tempAnoIngresso;

	arquivoAdvogado = fopen("./data/ADVOGADOS.DAT", "a+b");

	if (arquivoAdvogado == NULL)
	{
		printf("Erro ao abrir ADVOGADOS.DAT\n");
		system("pause");
		exit(0);
	};

	fwrite(&advogado, sizeof(Advogado), 1, arquivoAdvogado);

	fclose(arquivoAdvogado);

	printf("\nADVOGADO ADICIONADO NO SISTEMA COM SUCESSO!\n");
	gerarRelatorioAdv(&advogado);

	printf("\n--------------------------------------------------------\n");
}

int obterTodosAdvogados()
{
	int count = 0;
	arquivoAdvogado = fopen("./data/ADVOGADOS.DAT", "r+b");
	if (arquivoAdvogado == NULL)
	{
		printf("Erro ao abrir ADVOGADOS.DAT\n");
		system("pause");
		exit(0);
	};

	// Atenção:
	fseek(arquivoAdvogado, 0, SEEK_END);
	// Obtém a posição atual do ponteiro (tamanho do arquivo em bytes)
	long tamanhoArquivo = ftell(arquivoAdvogado);
	// Calcula o número de registros dividindo o tamanho do arquivo pelo tamanho de cada registro
	int numRegistros = tamanhoArquivo / sizeof(Advogado);
	// Volta o ponteiro de arquivo para o início do arquivo
	rewind(arquivoAdvogado);
	//

	advogados = (Advogado*)malloc(tamanhoArquivo);

	if (advogados == NULL) {
		printf("Erro interno. (FALHA NA ALOCAÇÃO DE MEMÓRIA)\n");
		system("pause");
		exit(0);
	}

	count = 0;

	while (!feof(arquivoAdvogado)) {
		fread(&advogado, sizeof(advogado), 1, arquivoAdvogado);
		if (!feof(arquivoAdvogado)) {
			advogados[count] = advogado;
			count++;
		}
	}

	fclose(arquivoAdvogado); //recuperou os dados e fechou

	ordenaAdvOab(advogados, count); //ordenou os dados

	arquivoAdvogado = fopen("./data/ADVOGADOS.DAT", "w+b"); //abriu o arquivo para sobreescrita
	fwrite(advogados, sizeof(Advogado), count, arquivoAdvogado); //sobreescreveu os dados ordenados
	fclose(arquivoAdvogado); //fechou o arquivo

	return count;
}


// Mostra todos os advogados cadastrados no sistema no console
void mostrarTodosAdvogados() {
	int count = obterTodosAdvogados();

	printf("\n-----------LISTA DE ADVOGADOS NO SISTEMA -------------\n");

	if (count == 0) {
		printf("\nNenhum advogado cadastrado no DJ.\n");
	}
	else {
		for (int i = 0; i < count; i++) {
			gerarRelatorioAdv(&advogados[i]);
			if (i < count - 1) {
				printf("\n          ¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨         ");
			}
		};
	}

	printf("\n--------------------------------------------------------\n");

	free(advogados); //desaloca a memória
}

Advogado retornarAdvogadoEspecifico(int oab) {
	int tamanho_vetor = obterTodosAdvogados();

	int inicio = 0, fim = tamanho_vetor - 1, meio;
	bool encontrado = false;

	//busca binária
	while (inicio <= fim)
	{
		meio = (inicio + fim) / 2;
		if (advogados[meio].oab == oab) {
			encontrado = true;
			return advogados[meio];
		}
		if (advogados[meio].oab > oab)
			fim = meio - 1;
		else
			inicio = meio + 1;
	}

	if (!encontrado) {
		printf("\nNenhum advogado encontrado com a OAB: %i\n", oab);
	}

	free(advogados);

	Advogado adv_invalido;

	adv_invalido.oab = -1;

	return adv_invalido;
}

//Insertion Sort
void ordenaAdvOab(Advogado* advogados, int tamanho_vetor) {
	int anterior;
	Advogado escolhido;

	for (int i = 1; i < tamanho_vetor; i++) {
		escolhido = advogados[i];
		anterior = i - 1;

		while ((anterior >= 0) && (advogados[anterior].oab > escolhido.oab)) {
			advogados[anterior + 1] = advogados[anterior];
			anterior = anterior - 1;
		};

		advogados[anterior + 1] = escolhido;
	}
}

void gerarRelatorioAdv(Advogado* advogado) {
	printf("\nOAB:              %i\n", advogado->oab);
	printf("Nome:             %s\n", advogado->nome);
	printf("CPF:              %s\n", advogado->cpf);
	printf("Especialidade:    %s\n", advogado->especialidade);
	printf("Número:           %s\n", advogado->numeroTelefone);
	printf("Email:            %s\n", advogado->email);
	printf("Ano de ingresso:  %i\n", advogado->anoIngresso);
}