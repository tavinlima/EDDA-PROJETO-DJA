#ifndef GLOBAL_H
#define GLOBAL_H

#include <stddef.h>

void gerarData(char* buffer, size_t size, int acresDias);

void limparBuffer();
void gerarDataAtual(char* buffer, size_t size);
void gerarDataAtualMaisUm(char* buffer, size_t size);
void gerarHoraAtual(char* buffer, size_t size);
int retornarDiaSemana();

#endif