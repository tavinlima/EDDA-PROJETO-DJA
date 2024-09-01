#ifndef ESTAGIARIO_H
#define ESTAGIARIO_H


typedef struct {
	int codeEstag;
	char nome[50];
	char numeroTelefone[13];
	char email[30];
	char tipo;
} Estagiario;


void adicionarEstagiario();
void gerarCodEstagiario(Estagiario *estagiario);
int retornarUltimoCodEstag();
int obterTodosOsEstagiarios();
void gerarRelatorioEstag(Estagiario* estagiario);
void mostrarTodosEstagiarios();
Estagiario retornarEstagiarioEspecifico(int codEstag);
#endif

