#ifndef CASOSATRIBUIDOS_H
#define CASOSATRIBUIDOS_H

//struct
typedef struct {
	char cpf[13];
	int codAtendimento;
	int codEstagiario;
	char dataAtribuicao[12];
	char horaAtribuicao[6];

} CasosAtribuidos;

void atribuirEstagiario();
void gerarRelatorioCasosAtribuidos(CasosAtribuidos *atribuicaoCaso);
//void listarCasosAtribuidos();

#endif