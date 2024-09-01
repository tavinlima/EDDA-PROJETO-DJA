#define _CRT_SECURE_NO_WARNINGS // desabilita alertas de unsafe

// Bibliotecas
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>

// Arquivos
#include "global.h"
#include "advogado.h"
#include "agenda.h"
#include "casosAtribuidos.h"
#include "triagem.h"
#include "estagiario.h"
#include "disponibilidade.h"
#include "controle.h"

void gerarMensagemBoasVindas();

void menuAdvogado();

void menuEstagiario();

void menuControle();

// Variáveis
int opcao = 1;

int main()
{
	setlocale(LC_ALL, ""); // Seta a localização para a padrão do sistema

	gerarMensagemBoasVindas();

	do
	{
		printf(" *********************************************\n");
		printf("*        Qual ação deseja realizar?           *\n");
		printf("*                                             *\n");
		printf("*   Digite:                                   *\n");
		printf("*   1 - Abrir menu do advogado                *\n");
		printf("*   2 - Abrir menu do estagiário              *\n");
		printf("*   3 - Abrir menu do controle                *\n");
		printf("*                                             *\n");
		printf("*   9 - Sair                                  *\n");
		printf("*                                             *\n");
		printf(" *********************************************\n");

		scanf("%i", &opcao);
		limparBuffer();

		switch (opcao)
		{
		case 1:
			menuAdvogado();
			break;
		case 2:
			menuEstagiario();
			break;
		case 3:
			menuControle();
			break;
		case 9:
			printf("Até breve!\n");
			exit(0);
		default:
			break;
		}

		limparBuffer();

	} while (opcao != 9);

	return 0;
}

// Funções auxiliares
void gerarMensagemBoasVindas()
{
	printf(" *********************************************\n");
	printf("*                                             *\n");
	printf("*      Bem-vindo ao Departamento Jurídico     *\n");
	printf("*    Departamento Jurídico XI de Agosto       *\n");
	printf("*                                             *\n");
	printf("*   Nosso compromisso é garantir acesso à     *\n");
	printf("*   justiça e proteção legal para quem        *\n");
	printf("*   mais precisa de apoio especializado.      *\n");
	printf("*                                             *\n");
	printf(" *********************************************\n");
}

void menuAdvogado()
{
	system("clear");
	printf(" *********************************************\n");
	printf("*                                             *\n");
	printf("*             Bem-vindo ao DJA!               *\n");
	printf("*    Departamento Jurídico XI de Agosto       *\n");
	printf("*                                             *\n");
	printf("*   	    	Menu - Advogado               *\n");
	printf("*   Digite:                                   *\n");
	printf("*   1 - Inserir advogados ao sistema          *\n");
	printf("*   2 - Listar todos os advogados             *\n");
	printf("*   3 - Inserir disponibilidades              *\n");
	printf("*   4 - Visualizar agendamentos               *\n");
	printf("*   5 - Realizar atendimento                  *\n");
	printf("*   6 - Visualizar histórico de agendamentos   *\n");
	printf("*                                             *\n");
	printf("*   9 - Voltar ao menu principal              *\n");
	printf("*                                             *\n");
	printf(" *********************************************\n");

	scanf("%i", &opcao);
	limparBuffer();

	switch (opcao)
	{
	case 1:
		adicionarAdvogado();
		break;
	case 2:
		mostrarTodosAdvogados();
		break;
	case 3:
		adicionarDisponibilidade();
		break;
	case 4:
		visualizarTodasFilas();
		break;
	case 5:
		processarAtendimento();
		break;
	case 6:
		mostrarHistoricoAgendamentos();
		break;
	case 9:
		system("clear");
		main();
	default:
		system("clear");
		main();
		break;
	}

	limparBuffer();
}

void menuEstagiario()
{
	system("clear");
	printf(" *********************************************\n");
	printf("*                                             *\n");
	printf("*             Bem-vindo ao DJA!               *\n");
	printf("*    Departamento Jurídico XI de Agosto       *\n");
	printf("*                                             *\n");
	printf("*   	    	Menu - Estagiário             *\n");
	printf("*   Digite:                                   *\n");
	printf("*   1 - Inserir estagiários ao sistema        *\n");
	printf("*   2 - Listar todos os estagiários           *\n");
	printf("*   3 - Agendar atendimento com adv           *\n");
	printf("*                                             *\n");
	printf("*   4 - Realizar triagem                      *\n");
	printf("*   5 - Visualizar todas as triagens          *\n");
	printf("*   6 - Atribuir estagiário                   *\n");
	printf("*                                             *\n");
	printf("*   9 - Voltar ao menu principal              *\n");
	printf("*                                             *\n");
	printf(" *********************************************\n");

	scanf("%i", &opcao);
	limparBuffer();

	switch (opcao)
	{
	case 1:
		adicionarEstagiario();
		break;
	case 2:
		mostrarTodosEstagiarios();
		break;
	case 3:
		agendarAtendimento();
		break;
	case 4:
		realizarTriagem();
		break;
	case 5:
		mostrarCasosEmAndamento();
		break;
	case 6:
		atribuirEstagiario();
		break;
	case 9:
		system("clear");
		main();
	default:
		system("clear");
		main();
		break;
	}

	limparBuffer();
}

void menuControle()
{
	system("clear");
	printf("\n");
	printf(" *********************************************\n");
	printf("*                                             *\n");
	printf("*             Bem-vindo ao DJA!               *\n");
	printf("*    Departamento Jurídico XI de Agosto       *\n");
	printf("*                                             *\n");
	printf("*   	    	Menu - Geral                  *\n");
	printf("*   Digite:                                   *\n");
	printf("*   1 - Atualizar/Inserir Controle dos casos  *\n");
	printf("*   2 - Visualizar controle dos casos         *\n");
	printf("*   3 - Mostrar todos os clientes             *\n");
	printf("*   4 - Mostrar cliente específico            *\n");
	printf("*                                             *\n");
	printf("*                                             *\n");
	printf("*   9 - Voltar ao menu principal              *\n");
	printf("*                                             *\n");
	printf(" *********************************************\n");

	scanf("%i", &opcao);
	limparBuffer();

	switch (opcao)
	{
	case 1:
		atualizarOuInserirMovimentacao();
		break;
	case 2:
		verTodasMovimentacoes();
		break;
	case 3:
		mostrarTodosClientes();
		break;
	case 4:
		retornarClienteEspecifico();
		break;
	case 9:
		system("clear");
		main();
	default:
		system("clear");
		main();
		break;
	}

	limparBuffer();
}