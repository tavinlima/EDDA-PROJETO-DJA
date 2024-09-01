#define _CRT_SECURE_NO_WARNINGS // desabilita alertas de unsafe

// Bibliotecas
#include <stdio.h>	 // Biblioteca padrão de entrada e saída
#include <stdbool.h> //Utilizado para verificações booleanas
#include <string.h>	 //inclui funções para copiar (strcpy), concatenar (strcat), comparar (strcmp)
#include <stdlib.h>

// Módulos
#include "triagem.h"
#include "global.h"
#include "estagiario.h"

// Variáveis globais
double salMinimo = 1412.36; // Variável que será utilizada no cálculo da renda familiar
FILE *arquivoCasos;			// Ponteiro para o arquivo CLIENTES.DAT, será utilizado para resgatar, gravar e atualizar os dados dos atendimentos
triagem atendimento;		// Variável que armazenará os dados do atendimento
triagem *clientes;
Estagiario estag; // Servirá para verificar se o estagiário é valido e se é atendente ou não.

void realizarTriagem()
{
	// Variáveis temporárias (Usadas para verificações de entrada do usuário)
	int tempCodEstag;
	char tempTipoPessoa;

	printf("\n----------- TRIAGEM DE CASO -------------\n");

	// Atribui um código do atendimento válido
	atendimento.codAtendimento = retornarCodAtendimento();

	// Gera a data e hora do atendimento
	gerarData(atendimento.data, sizeof(atendimento.data), 0);
	gerarHoraAtual(atendimento.horaAtendimento, sizeof(atendimento.horaAtendimento));

	// Enquanto o estagiário não for um atendente, o código do estagiário é solicitado novamente
	do
	{
		printf("Digite o código do estagiário: ");
		scanf("%d", &tempCodEstag);
		limparBuffer();
		estag = retornarEstagiarioEspecifico(tempCodEstag);
		if (estag.tipo != 'A')
			printf("Somente estagiários atendentes podem realizar a triagem.\n\n");
	} while (estag.tipo != 'A');
	// Atribui o código do estagiário ao atendimento
	atendimento.codEstagiario = tempCodEstag;

	printf("\nHorário que o atendido chegou (hh:mm): ");
	fgets(atendimento.horaChegada, sizeof(atendimento.horaChegada), stdin);
	atendimento.horaChegada[strcspn(atendimento.horaChegada, "\n")] = '\0';
	printf("\nFonte de informação do servico: ");
	fgets(atendimento.fonteInformacaoServico, sizeof(atendimento.fonteInformacaoServico), stdin);
	atendimento.fonteInformacaoServico[strcspn(atendimento.fonteInformacaoServico, "\n")] = '\0';

	// Verifica se o tipo de pessoa é válido (Física ou Jurídica)
	// Adicionar campo de CNPJ caso seja pessoa jurídica posteriormente
	do
	{
		printf("\nTipo de pessoa (F/J): ");
		tempTipoPessoa = getchar(); // O comportamento do getchar é de consumir o caracterer. Se não for capturado por uma variável, o valor será perdido.
		limparBuffer();
		if (tempTipoPessoa != 'F' && tempTipoPessoa != 'J')
			printf("Tipo de pessoa inválido. Digite F para pessoa física ou J para pessoa jurídica.\n");
	} while (tempTipoPessoa != 'F' && tempTipoPessoa != 'J');
	// Atribui o tipo de pessoa ao atendimento
	atendimento.tipoPessoa = tempTipoPessoa;

	printf("\nNome do atendido: ");
	fgets(atendimento.nome, sizeof(atendimento.nome), stdin);
	atendimento.nome[strcspn(atendimento.nome, "\n")] = '\0';
	printf("\nGênero: ");
	fgets(atendimento.genero, sizeof(atendimento.genero), stdin);
	atendimento.genero[strcspn(atendimento.genero, "\n")] = '\0';

	// Adicionar verificação (Só se quiser) qualquer coisa diferente de 'S' vai ser setado com false:
	// Se adicionar verificação, criar variavel temporaria para consumir o getchar
	printf("\nPertence a comunidade LGBTQIA+? (S/N): ");
	if (getchar() == 'S')
		atendimento.comLgbt = true;
	else
		atendimento.comLgbt = false;
	limparBuffer();

	printf("\nCor/Raça: ");
	fgets(atendimento.corRaca, sizeof(atendimento.corRaca), stdin);
	atendimento.corRaca[strcspn(atendimento.corRaca, "\n")] = '\0';
	printf("\nData de nascimento: ");
	fgets(atendimento.dataNascimento, sizeof(atendimento.dataNascimento), stdin);
	atendimento.dataNascimento[strcspn(atendimento.dataNascimento, "\n")] = '\0';
	printf("\nNacionalidade: ");
	fgets(atendimento.nacionalidade, sizeof(atendimento.nacionalidade), stdin);
	atendimento.nacionalidade[strcspn(atendimento.nacionalidade, "\n")] = '\0';
	printf("\nNaturalidade: ");
	fgets(atendimento.naturalidade, sizeof(atendimento.naturalidade), stdin);
	atendimento.naturalidade[strcspn(atendimento.naturalidade, "\n")] = '\0';

	do
	{
		printf("\nCPF: ");
		fgets(atendimento.cpf, sizeof(atendimento.cpf), stdin);
		atendimento.cpf[strcspn(atendimento.cpf, "\n")] = '\0';

		if (strlen(atendimento.cpf) != 11)
			printf("\nO CPF deve ter 11 dígitos.");

	} while (strlen(atendimento.cpf) != 11);

	do
	{
		printf("\nRG: ");
		fgets(atendimento.rg, sizeof(atendimento.rg), stdin);
		atendimento.rg[strcspn(atendimento.rg, "\n")] = '\0';

		if (strlen(atendimento.rg) != 9)
			printf("\nO RG deve ter 9 dígitos.");

	} while (strlen(atendimento.rg) != 9);

	printf("\nEndereço: ");
	fgets(atendimento.endereco, sizeof(atendimento.endereco), stdin);
	atendimento.endereco[strcspn(atendimento.endereco, "\n")] = '\0';
	printf("\nCelular: ");
	fgets(atendimento.celular, sizeof(atendimento.celular), stdin);
	atendimento.celular[strcspn(atendimento.celular, "\n")] = '\0';
	printf("\nPossui WhatsApp? (S/N): ");
	if (getchar() == 'S')
		atendimento.whatsapp = true;
	else
		atendimento.whatsapp = false;
	limparBuffer();
	printf("\nTelefone alternativo: ");
	fgets(atendimento.telAlternativo, sizeof(atendimento.telAlternativo), stdin);
	atendimento.telAlternativo[strcspn(atendimento.telAlternativo, "\n")] = '\0';
	printf("\nE-mail: ");
	fgets(atendimento.email, sizeof(atendimento.email), stdin);
	atendimento.email[strcspn(atendimento.email, "\n")] = '\0';
	printf("\nEscolaridade: ");
	fgets(atendimento.escolaridade, sizeof(atendimento.escolaridade), stdin);
	atendimento.escolaridade[strcspn(atendimento.escolaridade, "\n")] = '\0';

	// Se o atendido for desempregado, a profissão é setada como desempregado
	// Alterar se achar conveniente
	char resp;
	printf("\nO atendido está desempregado? (S/N) ");
	resp = getchar();
	if (resp == 'S')
	{
		strcpy(atendimento.situacaoProfissional, "Desempregado");
		strcpy(atendimento.profissao, "Desempregado");
	}
	else
	{
		strcpy(atendimento.situacaoProfissional, "Empregado");
		limparBuffer();
		printf("\nProfissão: ");
		fgets(atendimento.profissao, sizeof(atendimento.profissao), stdin);
		atendimento.profissao[strcspn(atendimento.profissao, "\n")] = '\0';
	}
	limparBuffer();

	printf("\nEstado civil: ");
	fgets(atendimento.estadoCivil, sizeof(atendimento.estadoCivil), stdin);
	atendimento.estadoCivil[strcspn(atendimento.estadoCivil, "\n")] = '\0';

	// Se o atendido possuir filhos, a quantidade de filhos morando com ele é solicitada.
	printf("\nPossui filhos? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.temFilhos = true;
		do
		{
			printf("\nQuantos filhos moram com você? ");
			scanf("%i", &atendimento.qtdFilhosMorando);
			limparBuffer();
		} while (atendimento.qtdFilhosMorando < 0 || atendimento.qtdFilhosMorando > 30);
	}
	else
	{
		atendimento.temFilhos = false;
		atendimento.qtdFilhosMorando = 0;
	}

	do
	{
		printf("\nTotal de membros da entidadade familiar: ");
		scanf("%i", &atendimento.totalMembrosFamilia);

		printf("\n\n %i \n\n", atendimento.totalMembrosFamilia);

		limparBuffer();
	} while (atendimento.totalMembrosFamilia < 1 || atendimento.totalMembrosFamilia > 30);

	////Adicionar verificação (Só se quiser) qualquer coisa diferente de 'S' vai ser setado com false:
	// Se adicionar verificação, criar variavel temporaria para consumir o getchar
	printf("\nPossui algum membro da família com deficiência? (S/N): ");
	if (getchar() == 'S')
		atendimento.membroFamiliaDeficiencia = true;
	else
		atendimento.membroFamiliaDeficiencia = false;
	limparBuffer();
	printf("\nPossui algum membro da família com Transtorno Global do Desenvolvimento (TGD)? (S/N): ");
	if (getchar() == 'S')
		atendimento.membroFamiliarTGD = true;
	else
		atendimento.membroFamiliarTGD = false;
	limparBuffer();
	printf("\nPossui algum membro da família idoso? (S/N): ");
	if (getchar() == 'S')
		atendimento.membroFamiliarIdoso = true;
	else
		atendimento.membroFamiliarIdoso = false;
	limparBuffer();
	printf("\nMembro familiar egresso prisional de até 4 meses (S/N): ");
	if (getchar() == 'S')
		atendimento.membroFamiliarEgressoPrisional = true;
	else
		atendimento.membroFamiliarEgressoPrisional = false;
	limparBuffer();

	printf("\nRecebe algum benefício assistencial do governo? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.recebeBeneficio = true;
		do
		{
			printf("\nQual o valor do benefício? ");
			scanf("%le", &atendimento.valorBeneficio);
			limparBuffer();

		} while (atendimento.valorBeneficio < 50 && atendimento.valorBeneficio > 5000);
	}
	else
	{
		atendimento.recebeBeneficio = false;
		atendimento.valorBeneficio = 0;
		limparBuffer();
	}

	do
	{
		printf("\nValor da renda mensal do assistido: ");
		scanf("%le", &atendimento.valorRenda);
		limparBuffer();
	} while (atendimento.valorRenda < 0 || atendimento.valorRenda > 1000000);

	do
	{
		printf("\nValor total da renda familiar: ");
		scanf("%le", &atendimento.totalRendaFamiliar);
		limparBuffer();
		if (atendimento.totalRendaFamiliar < 0 || atendimento.totalRendaFamiliar > 1000000)
			printf("Valor inválido. Digite um valor entre 0 e 1.000.000\n");
	} while (atendimento.totalRendaFamiliar < 0 || atendimento.totalRendaFamiliar > 1000000);

	printf("\nPossui gastos médicos? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.temGastosMedicos = true;
		printf("\nQual o valor dos gastos médicos? ");
		scanf("%le", &atendimento.valorGastosMedicos);
	}
	else
	{
		atendimento.temGastosMedicos = false;
		atendimento.valorGastosMedicos = 0;
	}
	limparBuffer();

	printf("\nPossui investimentos? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.possuiInvestimento = true;
		printf("\nQual o valor dos investimentos? ");
		scanf("%le", &atendimento.valorInvestimento);
	}
	else
	{
		atendimento.possuiInvestimento = false;
		atendimento.valorInvestimento = 0;
	}
	limparBuffer();

	printf("\nPossui casa própria? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.possuiCasaPropria = true;
		printf("\nQual o valor da casa própria? ");
		scanf("%le", &atendimento.valorCasaPropria);
	}
	else
	{
		atendimento.possuiCasaPropria = false;
		atendimento.valorCasaPropria = 0;
	}
	limparBuffer();

	printf("\nPossui apartamento? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.possuiApartamento = true;
		printf("\nQual o valor do apartamento? ");
		scanf("%le", &atendimento.valorApartamento);
	}
	else
	{
		atendimento.possuiApartamento = false;
		atendimento.valorApartamento = 0;
	}
	limparBuffer();

	printf("\nPossui terreno? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.possuiTerreno = true;
		printf("\nQual o valor do terreno? ");
		scanf("%le", &atendimento.valorTerreno);
	}
	else
	{
		atendimento.possuiTerreno = false;
		atendimento.valorTerreno = 0;
	}
	limparBuffer();

	printf("\nPossui imóvel comercial? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.possuiImovelComercial = true;
		printf("\nQual o valor do imóvel comercial? ");
		scanf("%le", &atendimento.valorImovelComercial);
	}
	else
	{
		atendimento.possuiImovelComercial = false;
		atendimento.valorImovelComercial = 0;
	}
	limparBuffer();

	printf("\nPossui veículo? (S/N): ");
	if (getchar() == 'S')
	{
		atendimento.possuiVeiculo = true;
		printf("\nQual o valor do veículo? ");
		scanf("%le", &atendimento.valorVeiculo);
	}
	else
	{
		atendimento.possuiVeiculo = false;
		atendimento.valorVeiculo = 0;
	}
	limparBuffer();

	printf("\nA demanda envolve violência doméstica? (S/N): ");
	if (getchar() == 'S')
		atendimento.demandaViolenciaDomestica = true;
	else
		atendimento.demandaViolenciaDomestica = false;
	limparBuffer();

	printf("\nResumo do caso: ");
	fgets(atendimento.resumoCaso, sizeof(atendimento.resumoCaso), stdin);
	atendimento.resumoCaso[strcspn(atendimento.resumoCaso, "\n")] = '\0';

	printf("\nRamo do direito: ");
	fgets(atendimento.ramoDireito, sizeof(atendimento.ramoDireito), stdin);
	atendimento.ramoDireito[strcspn(atendimento.ramoDireito, "\n")] = '\0';
	verificarPerfilEnquadrado(&atendimento);

	// Abre o arquivo CLIENTES.DAT
	arquivoCasos = fopen("./data/CLIENTES.DAT", "a+b");

	if (arquivoCasos == NULL)
	{
		printf("Erro ao abrir CLIENTES.DAT\n");
		system("pause");
		exit(0);
	};

	// Grava o atendimento no arquivo CLIENTES.DAT
	fwrite(&atendimento, sizeof(atendimento), 1, arquivoCasos);

	// Fecha o arquivo CLIENTES.DAT
	fclose(arquivoCasos);

	printf("\nCASO REGISTRADO NO SISTEMA COM SUCESSO!\n");

	// Exibe o relatório do atendimento
	gerarRelatorio(&atendimento);

	printf("\n------------------------------------------------------\n");
}

// Função que simplemente exibe todos os casos em andamento no console
void mostrarCasosEmAndamento()
{
	int count = 0;

	// Abre o arquivo CLIENTES.DAT
	arquivoCasos = fopen("./data/CLIENTES.DAT", "r+b");

	// Se por algum motivo o arquivo não puder ser aberto, arquivoCasos será NULL
	if (arquivoCasos == NULL)
	{
		printf("Erro ao abrir CLIENTES.DAT\n");
		system("pause");
		exit(0);
	};

	printf("\n----------- LISTA DE CLIENTES NO SISTEMA -------------\n");

	// Enquanto não chegar ao final do arquivo (feof - end of file)...
	while (!feof(arquivoCasos))
	{
		//...lê o próximo atendimento
		fread(&atendimento, sizeof(atendimento), 1, arquivoCasos);
		if (!feof(arquivoCasos))
		{
			// Soma o número de atendimentos
			count++;
			// Exibe o relatório do atendimento no console
			gerarRelatorio(&atendimento);
		}
		if (count == 0)
			printf("\nNenhum caso em andamento\n");
	}

	count = 0;

	fclose(arquivoCasos);

	printf("\n-------------------------------------------------------\n");

	system("pause");
}

// Função para retornar um caso específico. Pode ser utilizado para buscar uma atribuição de um estagiário, por exemplo.
// Recebe o código do atendimento como parâmetro
triagem retornarCasoEspecifico(int codAtendimento)
{
	bool encontrado = false;

	// Abre o arquivo CLIENTES.DAT
	arquivoCasos = fopen("./data/CLIENTES.DAT", "r+b");

	// Se por algum motivo o arquivo não puder ser aberto, arquivoCasos será NULL
	if (arquivoCasos == NULL)
	{
		printf("Erro ao abrir CLIENTES.DAT\n");
		system("pause");
		exit(0);
	};

	// Enquanto não chegar ao final do arquivo (feof - end of file)...
	while (!feof(arquivoCasos))

	{
		//...lê o próximo atendimento
		fread(&atendimento, sizeof(atendimento), 1, arquivoCasos);

		if (!feof(arquivoCasos))
		{
			// Se o código do atendimento for igual ao código passado como parâmetro
			// o loop é interrompido
			// o atendimento fica salvo na variável global atendimento
			if (atendimento.codAtendimento == codAtendimento)
			{
				encontrado = true;
				break;
			}
		}
	}

	// Se o atendimento não for encontrado, exibe mensagem de erro e encerra o programa
	if (!encontrado)
	{
		triagem triagem_inex;
		triagem_inex.codAtendimento = -1;
		return triagem_inex;
	}

	fclose(arquivoCasos);

	return atendimento;
}

// Função utilizada para gerar o código do atendimento e retorná-lo
int retornarCodAtendimento()
{
	triagem ultimoAtendimento; // Variável que armazenará oúltimo atendimento registrado

	// Abre o arquivo CLIENTES.DAT
	arquivoCasos = fopen("./data/CLIENTES.DAT", "r+b");

	// Se por algum motivo o arquivo não puder ser aberto, arquivoCasos será sempre NULL
	// Causas mais comuns : arquivo não existe, caminho do arquivo incorreto, arquivo aberto em outro local
	if (arquivoCasos == NULL)
	{
		printf("Erro ao abrir CLIENTES.DAT\n");
		system("pause");
		// O programa será encerrado completamente. **Pode ser utilizada outra abordagem.
		exit(0);
	};

	// Passando pelo IF anterior, o arquivo foi aberto com sucesso.
	// fseek é usada para mover o cursor de leitura/escrita para uma posição específica em um arquivo, permitindo saltar para diferentes partes do arquivo.
	fseek(arquivoCasos, -1 * sizeof(triagem), SEEK_END);
	// com os parametros acima, o cursor foi movido para a última linha do arquivo CLIENTES.DAT

	// fread é usada para ler um bloco de dados de um arquivo
	// com o cursos movido para a última linha do arquivo, a função fread lê o último atendimento registrado
	fread(&ultimoAtendimento, sizeof(triagem), 1, arquivoCasos);
	if (feof(arquivoCasos))
		ultimoAtendimento.codAtendimento = 0;
	// Se não houver atendimentos registrados, o código do atendimento será 0 provisoriamente

	// Fecha o arquivo CLIENTES.DAT
	fclose(arquivoCasos);

	// Retorna o código do último atendimento registrado (ou zero) + 1
	return ultimoAtendimento.codAtendimento + 1;
};

void gerarRelatorio(triagem *atendimento)
{
	printf("\nCod. Atendimento         : %d\n", atendimento->codAtendimento);
	printf("Cod. Estagiário Atendente  : %d\n", atendimento->codEstagiario);
	printf("Data                       : %s\n", atendimento->data);
	printf("Hora de Atendimento        : %s\n", atendimento->horaAtendimento);
	printf("Hora de Chegada            : %s\n", atendimento->horaChegada);
	printf("Fonte de Informação        : %s\n", atendimento->fonteInformacaoServico);
	printf("Pessoa Fisica/Juridica     : %c\n", atendimento->tipoPessoa);
	printf("Nome do(a) Atendido        : %s\n", atendimento->nome);
	printf("Gênero                     : %s\n", atendimento->genero);
	printf("Comunidade LGBTQIA+        : %s\n", atendimento->comLgbt ? "Sim" : "Não");
	printf("Cor/Raça                   : %s\n", atendimento->corRaca);
	printf("Data de Nascimento         : %s\n", atendimento->dataNascimento);
	printf("Nacionalidade              : %s\n", atendimento->nacionalidade);
	printf("Naturalidade               : %s\n", atendimento->naturalidade);
	printf("CPF                        : %s\n", atendimento->cpf);
	printf("RG                         : %s\n", atendimento->rg);
	printf("Endereço                   : %s\n", atendimento->endereco);
	printf("Celular                    : %s\n", atendimento->celular);
	printf("WhatsApp                   : %s\n", atendimento->whatsapp ? "Sim" : "Não");
	printf("Tel. Alternativo           : %s\n", atendimento->telAlternativo);
	printf("E-mail                     : %s\n", atendimento->email);
	printf("Escolaridade               : %s\n", atendimento->escolaridade);
	printf("Profissão                  : %s\n", atendimento->profissao);
	printf("Situação Profissional      : %s\n", atendimento->situacaoProfissional);
	printf("Estado Civil               : %s\n", atendimento->estadoCivil);
	printf("Possui Filhos              : %s\n", atendimento->temFilhos ? "Sim" : "Não");
	if (atendimento->temFilhos)
		printf("Qtd. Filhos Morando        : %d\n", atendimento->qtdFilhosMorando);
	printf("Total Membros Família      : %d\n", atendimento->totalMembrosFamilia);
	printf("Membro com Deficiência     : %s\n", atendimento->membroFamiliaDeficiencia ? "Sim" : "Não");
	printf("Membro com TGD             : %s\n", atendimento->membroFamiliarTGD ? "Sim" : "Não");
	printf("Membro Idoso               : %s\n", atendimento->membroFamiliarIdoso ? "Sim" : "Não");
	printf("Membro Egresso Prisional   : %s\n", atendimento->membroFamiliarEgressoPrisional ? "Sim" : "Não");
	printf("Recebe Benefício           : %s\n", atendimento->recebeBeneficio ? "Sim" : "Não");
	if (atendimento->recebeBeneficio)
		printf("Valor do Benefício         : %f\n", atendimento->valorBeneficio);
	printf("Valor da Renda             : %f\n", atendimento->valorRenda);
	printf("Total Renda Familiar       : %f\n", atendimento->totalRendaFamiliar);
	printf("Possui Gastos Médicos      : %s\n", atendimento->temGastosMedicos ? "Sim" : "Não");
	if (atendimento->temGastosMedicos)
		printf("Valor Gastos Médicos       : %f\n", atendimento->valorGastosMedicos);
	printf("Possui Investimento        : %s\n", atendimento->possuiInvestimento ? "Sim" : "Não");
	if (atendimento->possuiInvestimento)
		printf("Valor Investimento         : %f\n", atendimento->valorInvestimento);
	printf("Possui Casa Própria        : %s\n", atendimento->possuiCasaPropria ? "Sim" : "Não");
	if (atendimento->possuiCasaPropria)
		printf("Valor Casa Própria         : %f\n", atendimento->valorCasaPropria);
	printf("Possui Apartamento         : %s\n", atendimento->possuiApartamento ? "Sim" : "Não");
	if (atendimento->possuiApartamento)
		printf("Valor Apartamento          : %f\n", atendimento->valorApartamento);
	printf("Possui Terreno             : %s\n", atendimento->possuiTerreno ? "Sim" : "Não");
	if (atendimento->possuiTerreno)
		printf("Valor Terreno              : %f\n", atendimento->valorTerreno);
	printf("Possui Imóvel Comercial    : %s\n", atendimento->possuiImovelComercial ? "Sim" : "Não");
	if (atendimento->possuiImovelComercial)
		printf("Valor Imóvel Comercial     : %f\n", atendimento->valorImovelComercial);
	printf("Possui Veículo             : %s\n", atendimento->possuiVeiculo ? "Sim" : "Não");
	if (atendimento->possuiVeiculo)
		printf("Valor Veículo              : %f\n", atendimento->valorVeiculo);
	printf("Demanda Violência Doméstica: %s\n", atendimento->demandaViolenciaDomestica ? "Sim" : "Não");
	printf("Resumo do Caso             : %s\n", atendimento->resumoCaso);
	printf("Ramo do Direito            : %s\n", atendimento->ramoDireito);
	printf("Perfil Enquadrado          : %s\n", atendimento->perfilEnquadrado ? "Sim" : "Não");
}

triagem retornarClienteEspecifico()
{
    int tamanho_vetor = obterTodosClientes();
    char cpf[12];
    int inicio = 0, fim = tamanho_vetor - 1, meio;
    bool encontrado = false;

    // Loop para garantir que o CPF inserido tenha 11 dígitos
    do
    {
        printf("\nCPF: ");
        fgets(cpf, sizeof(cpf), stdin);
        limparBuffer();
        cpf[strcspn(cpf, "\n")] = '\0';

        if (strlen(cpf) != 11)
            printf("\nO CPF deve ter 11 dígitos.");
    } while (strlen(cpf) != 11);

    // Busca binária
    while (inicio <= fim)
    {
        meio = (inicio + fim) / 2;
        int result = strcmp(clientes[meio].cpf, cpf);

        if (result == 0)
        {
            encontrado = true;
            gerarRelatorio(&clientes[meio]);
            return clientes[meio];
        }
        else if (result > 0)
        {
            fim = meio - 1;
        }
        else
        {
            inicio = meio + 1;
        }
    }

    if (!encontrado)
    {
        printf("\nNenhum cliente encontrado com o CPF: %s\n", cpf);
    }

    free(clientes);

    // Retorna um cliente inválido se não encontrado
    triagem cliente_invalido;
    strcpy(cliente_invalido.cpf, "-1");

    return cliente_invalido;
}

// Função para verificar se o perfil do atendido se enquadra nos critérios de atendimento
// Recebe o ponteiro do atendimento como parâmetro
void verificarPerfilEnquadrado(triagem *atendimento)
{
	// Se a renda familiar for menor ou igual a 3 salários mínimos, o perfil é enquadrado
	if (atendimento->totalRendaFamiliar <= 3 * salMinimo)
	{
		printf("Perfil enquadrado por ter renda familiar menor ou igual que tres salarios minimos\n");
		atendimento->perfilEnquadrado = true;
	}
	// Se a renda familiar for menor ou igual a 4 salários mínimos...
	else if (atendimento->totalRendaFamiliar <= 4 * salMinimo)
	{
		printf("Renda familiar menor ou igual que 4 salarios minimos. Verificando outras condições\n");
		// o perfil é enquadrado se:

		// o número de membros da família for maior que 5
		if (atendimento->totalMembrosFamilia > 5)
		{
			printf("Número de membros da familia maior que 5. Perfil enquadarado\n");
			atendimento->perfilEnquadrado = true;
		}
		// ou se o atendido tiver gastos médicos
		else if (atendimento->temGastosMedicos)
		{
			printf("Tem gastos medicos. Perfil enquadarado\n");
			atendimento->perfilEnquadrado = true;
		}
		// ou se o atendido possui membro da família com deficiência ou TGD
		else if (atendimento->membroFamiliaDeficiencia || atendimento->membroFamiliarTGD)
		{
			printf("Membro da familia com deficiencia ou TGD. Perfil enquadarado\n");
			atendimento->perfilEnquadrado = true;
		}
		// ou se o atendido possui membro da família idoso ou egresso prisional e o número de membros da família for maior ou igual a 4.
		else if ((atendimento->membroFamiliarIdoso || atendimento->membroFamiliarEgressoPrisional) && atendimento->totalMembrosFamilia >= 4)
		{
			printf("Membro da familia idoso ou egresso prisional com mais de 4 membros. Perfil enquadarado\n");
			atendimento->perfilEnquadrado = true;
		}
		// Se nenhuma das condições acima for atendida, o perfil nãp é enquadrado
		else
		{
			printf("Perfil NÃO enquadrado.\n");
			atendimento->perfilEnquadrado = false;
		}
	}
}

int obterTodosClientes()
{
	int count = 0;
	arquivoCasos = fopen("./data/CLIENTES.DAT", "r+b");
	if (arquivoCasos == NULL)
	{
		printf("Erro ao abrir CLIENTES.DAT\n");
		system("pause");
		exit(0);
	};

	// Atenção:
	fseek(arquivoCasos, 0, SEEK_END);
	// Obtém a posição atual do ponteiro (tamanho do arquivo em bytes)
	long tamanhoArquivo = ftell(arquivoCasos);
	// Calcula o número de registros dividindo o tamanho do arquivo pelo tamanho de cada registro
	int numRegistros = tamanhoArquivo / sizeof(triagem);
	// Volta o ponteiro de arquivo para o início do arquivo
	rewind(arquivoCasos);
	//

	clientes = (triagem *)malloc(tamanhoArquivo);

	if (clientes == NULL)
	{
		printf("Erro interno. (FALHA NA ALOCAÇÃO DE MEMÓRIA)\n");
		system("pause");
		exit(0);
	}

	count = 0;

	while (!feof(arquivoCasos))
	{
		fread(&atendimento, sizeof(triagem), 1, arquivoCasos);
		if (!feof(arquivoCasos))
		{
			clientes[count] = atendimento;
			count++;
		}
	}

	fclose(arquivoCasos); // recuperou os dados e fechou

	ordenaClientes(clientes, count); // ordenou os dados

	arquivoCasos = fopen("./data/CLIENTES.DAT", "w+b");		// abriu o arquivo para sobreescrita
	fwrite(clientes, sizeof(triagem), count, arquivoCasos); // sobreescreveu os dados ordenados
	fclose(arquivoCasos);									// fechou o arquivo

	return count;
}
void ordenaClientes(triagem *clientes, int tamanho_vetor)
{
	int anterior;
	triagem escolhido;

	for (int i = 1; i < tamanho_vetor; i++)
	{
		escolhido = clientes[i];
		anterior = i - 1;

		while ((anterior >= 0) && (clientes[anterior].codAtendimento > escolhido.codAtendimento))
		{
			clientes[anterior + 1] = clientes[anterior];
			anterior = anterior - 1;
		};

		clientes[anterior + 1] = escolhido;
	}
}

void mostrarTodosClientes()
{
	int count = obterTodosClientes();

	printf("\n-----------LISTA DE CLIENTES NO SISTEMA -------------\n");

	if (count == 0)
	{
		printf("\nNenhum cliente cadastrado no DJ.\n");
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			gerarRelatorio(&clientes[i]);
			if (i < count - 1)
			{
				printf("\n          ¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨         ");
			}
		};
	}

	printf("\n--------------------------------------------------------\n");

	free(clientes); // desaloca a memória
}