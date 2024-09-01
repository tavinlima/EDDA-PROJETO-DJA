#ifndef TRIAGEM_H
#define TRIAGEM_H
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>


typedef struct {
    int codAtendimento; // Automático
    int codEstagiario;
    char data[11]; // Automático
    char horaAtendimento[6]; // Automático
    char horaChegada[7];
    char fonteInformacaoServico[30];
    char tipoPessoa; // F ou J
    char nome[30];
    char genero[15];
    bool comLgbt;
    char corRaca[15];
    char dataNascimento[12];
    char nacionalidade[20];
    char naturalidade[20];
    char cpf[13];
    char rg[11];
    char endereco[80];
    char celular[14];
    bool whatsapp;
    char telAlternativo[14];
    char email[50];
    char escolaridade[40];
    char situacaoProfissional[20];
    char profissao[40];
    char estadoCivil[20];
    bool temFilhos;
    int qtdFilhosMorando;
    int totalMembrosFamilia;
    bool membroFamiliaDeficiencia;
    bool membroFamiliarTGD;
    bool membroFamiliarIdoso;
    bool membroFamiliarEgressoPrisional;
    bool recebeBeneficio;
    double valorBeneficio;
    double valorRenda;
    double totalRendaFamiliar;
    bool temGastosMedicos;
    double valorGastosMedicos;
    bool possuiInvestimento;
    double valorInvestimento;
    bool possuiCasaPropria;
    double valorCasaPropria;
    bool possuiApartamento;
    double valorApartamento;
    bool possuiTerreno;
    double valorTerreno;
    bool possuiImovelComercial;
    double valorImovelComercial;
    bool possuiVeiculo;
    double valorVeiculo;
    bool demandaViolenciaDomestica;
    char resumoCaso[500];
    char ramoDireito[20];
    bool perfilEnquadrado;
} triagem;

void realizarTriagem();
void mostrarCasosEmAndamento();
triagem retornarCasoEspecifico(int codAtendimento);
int retornarCodAtendimento();
void gerarRelatorio(triagem* atendimento);
void gerarMensagemBoasVindas();
void verificarPerfilEnquadrado(triagem* atendimento);
void mostrarTodosClientes();
int obterTodosClientes();
void ordenaClientes(triagem* clientes, int tamanho_vetor);
triagem retornarClienteEspecifico();


#endif
