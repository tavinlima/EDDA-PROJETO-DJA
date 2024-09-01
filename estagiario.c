#define _CRT_SECURE_NO_WARNINGS //desabilita alertas de unsafe

//Módulos
#include "estagiario.h"
#include "global.h"

//bibliotecas
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Variáveis globais
Estagiario estagiario; //Vai armazenar o estagiário
Estagiario* estagiarios; //Vai armazenar o array de estagiários
FILE* arquivo; //Ponteiro para o arquivo



void adicionarEstagiario() {
	//Variáveis temporárias
	char tempTipoEstagiario;

	//Vai setar o código do estagiário diretamente.
	//Diferente da triagem, estou passando a referência do estagiário.
	//Melhor abordagem que a usada no módulo de triagem, mas mesmo efeito.
	gerarCodEstagiario(&estagiario);

	printf("\n-----------INSERÇÃO DE ESTAGIÁRIO NO SISTEMA -------------\n");

	//Pede para inserir os dados do estagiário
	printf("\nInsira o nome do estagiário: ");
	fgets(estagiario.nome, sizeof(estagiario.nome), stdin);
	estagiario.nome[strcspn(estagiario.nome, "\n")] = '\0';
	printf("\nInsira o número de telefone do estagiário: ");
	fgets(estagiario.numeroTelefone, sizeof(estagiario.numeroTelefone), stdin);
	estagiario.numeroTelefone[strcspn(estagiario.numeroTelefone, "\n")] = '\0';
	printf("\nInsira o email do estagiário: ");
	fgets(estagiario.email, sizeof(estagiario.email), stdin);
	estagiario.email[strcspn(estagiario.email, "\n")] = '\0';

	// Validação do tipo de estagiário, enquanto não for A, P ou V, pede para inserir novamente
	do {
		printf("\nInsira o tipo de estagiário: ");
		tempTipoEstagiario = getchar();
		limparBuffer();
	} while (tempTipoEstagiario != 'A' && tempTipoEstagiario != 'P' && tempTipoEstagiario != 'V');
	// Atribui o tipo de estagiário validado ao estagiário
	estagiario.tipo = tempTipoEstagiario;

	//Abre, verifica se o arquivo foi aberto, escreve e fecha o arquivo
	fopen("./data/ESTAGIARIOS.DAT", "a+b");
	if (arquivo == NULL) {
		printf("Erro ao abrir ESTAGIARIOS.DAT\n");
		system("pause");
		exit(0);
	}
	//Escreve o estagiário no arquivo
	fwrite(&estagiario, sizeof(Estagiario), 1, arquivo);
	//Fecha o arquivo
	fclose(arquivo);

	printf("\nESTAGIÁRIO ADICIONADO NO SISTEMA COM SUCESSO!\n");
	//Imprime o relatório do estagiário adicionado
	gerarRelatorioEstag(&estagiario);

	printf("\n--------------------------------------------------------\n");
}

//Retorna o último código de estagiário cadastrado.
//Mesma lógica do código de triagem/atendimento.
int retornarUltimoCodEstag() {
	//Declara a variável que vai armazenar o último estagiário do arquivo
	Estagiario ultimoEstagiario;
	//Abre o arquivo
	arquivo = fopen("./data/ESTAGIARIOS.DAT", "a+b");
	//Se não conseguir abrir, exibe a mensagem de erro e encerra o programa
	if (arquivo == NULL) {
		printf("Não foi possível abrir ESTAGIARIOS.DAT");
		system("pause");
		exit(0);
	}
	//Vai para o final do arquivo
	fseek(arquivo, -1 * sizeof(Estagiario), SEEK_END);
	//Lê o último estagiário e armazena na variável ultimoEstagiario
	fread(&ultimoEstagiario, sizeof(Estagiario), 1, arquivo);
	//Se o arquivo estiver vazio, seta o código do estagiário como 0
	if (feof(arquivo)) ultimoEstagiario.codeEstag = 0;
	//Fecha o arquivo
	fclose(arquivo);
	//Retorna o código do último estagiário (ou zero) 
	return ultimoEstagiario.codeEstag;
}

//Basicamente seta o código do estagiário como o último código de estagiário + 1
void gerarCodEstagiario(Estagiario* estagiario) {
	//chama a função que retorna o último código de estagiário
	int ultimoCod = retornarUltimoCodEstag();
	//Seta o código do estagiário como o último código de estagiário + 1
	estagiario->codeEstag = ultimoCod + 1;
}

//Armazena todos os estagiários cadastrados que estão no arquivo ESTAGIARIOS.DAT em 
//um array de Estagiario
int obterTodosOsEstagiarios()
{
	//Chama a função que retorna o último código de estagiário para descobrir quantos registros existem
	int ultimoCod = retornarUltimoCodEstag();

	int count = 0;

	//Abre o arquivo
	arquivo = fopen("./data/ESTAGIARIOS.DAT", "r+b");
	//Se não conseguir abrir, exibe a mensagem de erro e encerra o programa
	if (arquivo == NULL)
	{
		printf("Erro ao abrir ESTAGIARIOS.DAT\n");
		system("pause");
		exit(0);
	};

	//Faz a alocação de memória para o array de estagiários
	estagiarios = (Estagiario*)malloc(ultimoCod * sizeof(Estagiario));

	//Se não conseguir alocar a memória, exibe a mensagem de erro e encerra o programa
	if (estagiarios == NULL) {
		printf("Erro interno. (FALHA NA ALOCAÇÃO DE MEMÓRIA)\n");
		system("pause");
		exit(0);
	}

	//Enquanto não chegar no final do arquivo, lê os registros e armazena no array de estagiários
	while (!feof(arquivo)) {
		fread(&estagiario, sizeof(estagiario), 1, arquivo);
		if (!feof(arquivo)) {
			estagiarios[count] = estagiario;
			count++;
		}
	}

	//Fecha o arquivo
	fclose(arquivo);

	//retorna a quantidade de estagiários cadastrados para ser usada em outras funções
	return count;
}

//Utilizada para mostrar todos os estagiários cadastrados no sistema no console
void mostrarTodosEstagiarios() {
	//Chama a função que armazena todos os estagiários cadastrados no array de estagiários e retorna a quantidade de estagiários
	int count = obterTodosOsEstagiarios();

	printf("\n-----------LISTA DE ESTAGIÁRIO NO SISTEMA -------------\n");

	if (count == 0) {
		printf("Nenhum estagiário cadastrado no DJ.\n\n");
	}
	else {
		//Se houver estagiários cadastrados, imprime o relatório de cada um por meio de um for básico
		for (int i = 0; i < count; i++) {
			gerarRelatorioEstag(&estagiarios[i]);
			if (i < count - 1) {
				printf("\n          ¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨         ");
			}
		};
	}

	printf("\n--------------------------------------------------------\n");

	//Desaloca a memória.
		//Se não fizer isso após a execução da função, o programa vai consumir memória desnecessariamente.
	free(estagiarios);
}

//Retorna um estagiário específico com base no código do estagiário
Estagiario retornarEstagiarioEspecifico(int codEstag) {
	//Chama a função que armazena todos os estagiários cadastrados no array de estagiários e retorna a quantidade de estagiários
	int tamanho_vetor = obterTodosOsEstagiarios();

	//Declara as variáveis de busca binária
	int inicio = 0, fim = tamanho_vetor - 1, meio;

	//Variável de controle
	bool encontrado = false;

	//Realiza a busca binária
	while (inicio <= fim)
	{
		meio = (inicio + fim) / 2;
		if (estagiarios[meio].codeEstag == codEstag) {
			encontrado = true;
			//Encontrou, retornou.
			return estagiarios[meio];
		}
		if (estagiarios[meio].codeEstag > codEstag)
			fim = meio - 1;
		else
			inicio = meio + 1;
	}

	//Se não encontrar o estagiário, exibe a mensagem.
	if (!encontrado) {
		printf("Nenhum estagiário encontrado com o código: %d\n\n", codEstag);
		system("pause");
	}

	//Desaloca a memória.
	//Se não fizer isso após a execução da função, o programa vai consumir memória desnecessariamente.
	free(estagiarios);

	//Retorna um estagiário inválido para verificação em outras partes do código
	//Talvez não seja a melhor abordagem, mas é funcional.
	Estagiario estag_invalido;
	estag_invalido.codeEstag = -1;
	return estag_invalido;
}

//Gera o relatório do estagiário
void gerarRelatorioEstag(Estagiario* estagiario) {
	printf("\nCódigo:         %i\n", estagiario->codeEstag);
	printf("Nome:           %s\n", estagiario->nome);
	printf("Número:         %s\n", estagiario->numeroTelefone);
	printf("Email:          %s\n", estagiario->email);

	switch (estagiario->tipo)
	{
	case 'A':
		printf("Tipo:           Atendente\n");
		break;
	case 'P':
		printf("Tipo:           Plantonista\n");
		break;
	case 'V':
		printf("Tipo:           Vareiro\n");
		break;
	default:
		break;
	}
}