#ifndef ADVOGADO_H
#define ADVOGADO_H

//struct
typedef struct {
	int oab;
	char nome[40];
	char especialidade[20];
	char cpf[13];
	char email[30];
	char numeroTelefone[15];
	int anoIngresso;

} Advogado;

void adicionarAdvogado();
void mostrarTodosAdvogados();
int obterTodosAdvogados();
void gerarRelatorioAdv(Advogado *advogado);
Advogado retornarAdvogadoEspecifico(int oab);
void ordenaAdvOab(Advogado* advogados, int tamanho_vetor);

#endif