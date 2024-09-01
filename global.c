#define _CRT_SECURE_NO_WARNINGS //desabilita alertas de funções consideradas inseguras pelo compilador

//Bibliotecas 
#include <stddef.h>
#include <stdio.h>
#include <time.h> // Utilizado para gerar data e hora atuais no programa
#include <stdlib.h>
#include <string.h>

//Módulos
#include "global.h"

//Função para limpar buffer do teclado
//Funcionamento básico:
/*"Enquanto o buffer possuir caracteres diferentes de \n (enter do teclado) e não chegar ao final do arquivo (EOF)
a variável c vai consumir os caracteres do buffer por meio do getchar(), então o buffer será limpo."*/

void limparBuffer() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

//Link do vídeo que explica a função limparBuffer: https://www.youtube.com/watch?v=Jv8eK0b1FHQ


//Função para gerar data atual, ou somar dias à data atual
//Parâmetros da função:
//buffer: propriedade que vai armazenar a data. Exemplo: agenda.dtInscricao
//size: tamanho do buffer (sizeof da propriedade) Exemplo: sizeof(agenda.dtInscricao)
//acresDias: quantidade de dias que deseja acrescentar na data atual. Exemplo: 1. Pode ser útil para gerar a data de amanhã.
//Em caso de gerar a data sem acrescentar dias, basta passar 0 como parâmetro.
//Exemplo final: gerarData(agenda.dtInscricao, sizeof(agenda.dtInscricao), 1);
//A função abaixo gera a data e armazena na propriedade desejada, sem necessidade de mais código.
void gerarData(char* buffer, size_t size, int acresDias) {

	time_t t;
	struct tm* tm_info;
	time(&t);
	tm_info = localtime(&t);
	tm_info->tm_mday += acresDias;
	mktime(tm_info);
	strftime(buffer, size, "%d/%m/%Y", tm_info);
}

//Parâmetros da função:
//buffer: propriedade que vai armazenar a hora. Exemplo: agenda.dtInscricao
//size: tamanho do buffer (sizeof da propriedade) Exemplo: sizeof(agenda.dtInscricao)
//Exemplo final: gerarHoraAtual(agenda.horaInscricao, sizeof(agenda.horaInscricao));
//A função abaixo gera a hora e armazena na propriedade desejada, sem necessidade de mais código.
void gerarHoraAtual(char* buffer, size_t size) {

	time_t t;
	struct tm* tm_info;
	time(&t);
	tm_info = localtime(&t);
	strftime(buffer, size, "%H:%M", tm_info);
}

//Função para retornar o dia da semana
//Retorna um inteiro que representa o dia da semana, sendo:
//0 - Domingo, 1 - Segunda-feira, 2 - Terça-feira, 3 - Quarta-feira, 4 - Quinta-feira, 5 - Sexta-feira, 6 - Sábado
int retornarDiaSemana() {
	time_t t;
	struct tm* tm_info;
	time(&t);
	tm_info = localtime(&t);
	return tm_info->tm_wday;
}
