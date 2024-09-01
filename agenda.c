#include "agenda.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

// Módulos
#include "global.h"
#include "advogado.h"
#include "estagiario.h"
#include "disponibilidade.h"

// Dicionário de filas de advogados
static GHashTable *filasAdvogados = NULL;

FILE *arquivoAgenda;
Agenda agenda;
Agenda *agendas;

// Inicializa uma fila para o advogado com o número da OAB fornecido
void inicializarFila(int oab)
{
    if (filasAdvogados == NULL)
    {
        filasAdvogados = g_hash_table_new(g_int_hash, g_int_equal);
    }
    if (!g_hash_table_contains(filasAdvogados, &oab))
    {
        int *key = malloc(sizeof(int));
        if (key == NULL)
        {
            printf("Erro ao alocar memória para a chave da fila.\n");
            exit(1);
        }
        *key = oab;
        GQueue *novaFila = g_queue_new();
        g_hash_table_insert(filasAdvogados, key, novaFila);
    }
}

// Enfileira um agendamento na fila correspondente ao advogado
void enfileirar(Agenda novoAgendamento)
{
    GQueue *fila = g_hash_table_lookup(filasAdvogados, &novoAgendamento.oab);
    if (fila == NULL)
    {
        printf("Fila para advogado com OAB %i não encontrada.\n", novoAgendamento.oab);
        return;
    }
    Agenda *novoAgendamentoPtr = malloc(sizeof(Agenda));
    if (novoAgendamentoPtr == NULL)
    {
        printf("Erro ao alocar memória para novo agendamento.\n");
        return;
    }
    *novoAgendamentoPtr = novoAgendamento;
    g_queue_push_tail(fila, novoAgendamentoPtr);
}

// Verifica se a fila do advogado com o número da OAB fornecido está vazia
bool filaVazia(int oab)
{
    GQueue *fila = g_hash_table_lookup(filasAdvogados, &oab);
    if (fila == NULL)
    {
        printf("Fila para advogado com OAB %i não encontrada.\n", oab);
        return true;
    }
    return g_queue_is_empty(fila);
}

// Retira um agendamento da fila correspondente ao advogado
Agenda *retirarDaFila(int oab)
{
    GQueue *fila = g_hash_table_lookup(filasAdvogados, &oab);
    if (fila == NULL)
    {
        printf("Fila para advogado com OAB %i não encontrada.\n", oab);
        return NULL;
    }
    if (filaVazia(oab))
    {
        printf("A fila do advogado com OAB %i está vazia.\n", oab);
        return NULL;
    }
    return (Agenda *)g_queue_pop_head(fila);
}

// Visualiza os agendamentos na fila do advogado com o número da OAB fornecido
void visualizarFila(int oab)
{
    GQueue *fila = g_hash_table_lookup(filasAdvogados, &oab);
    if (fila == NULL)
    {
        printf("Fila para advogado com OAB %i não encontrada.\n", oab);
        return;
    }
    if (filaVazia(oab))
    {
        printf("A fila do advogado com OAB %i está vazia.\n", oab);
        return;
    }
    GList *iter = fila->head;
    while (iter != NULL)
    {
        Agenda *agendamento = (Agenda *)iter->data;
        gerarRelatorioAgenda(agendamento);
        iter = iter->next;
    }
}

// Visualiza todas as filas
void visualizarTodasFilas()
{
    GList *keys = g_hash_table_get_keys(filasAdvogados);
    for (GList *iter = keys; iter != NULL; iter = iter->next)
    {
        int *oab = (int *)iter->data;
        printf("\n---- Fila do Advogado com OAB %i ----\n", *oab);
        visualizarFila(*oab);
    }
    g_list_free(keys);
}

// Função para processar atendimento (não modificada)
void processarAtendimento()
{
    Advogado adv;
    int tempOab;
    char areaDireito[30];
    int oab;

    // Mostra relatório de advogados disponíveis para o dia seguinte
    mostrarRelatorioAdvDis();

    // Enquanto não encontrar um advogado com a OAB informada, pede para inserir novamente
    do
    {
        printf("\nInsira o número da OAB do advogado que deseja realizar o atendimento: ");
        scanf("%i", &tempOab);
        limparBuffer();
        adv = retornarAdvogadoEspecifico(tempOab);
    } while (adv.oab == -1);

    strcpy(areaDireito, adv.especialidade);
    oab = adv.oab;

    adicionarObservacaoAtendimento();

    Agenda *agendamento = retirarDaFila(oab);
    if (agendamento != NULL)
    {
        printf("Processando agendamento:\n");
        gerarRelatorioAgenda(agendamento);

        // Não se esqueça de liberar a memória depois de processar o agendamento
        free(agendamento);
    }
}

// Função para agendar um atendimento e enfileirá-lo na fila do advogado
void agendarAtendimento()
{
    int tempOab;
    Advogado adv;
    int diaSemana = retornarDiaSemana(); // Supondo que retorna o dia da semana atual
    int codEstag;
    Estagiario estag;
    char tempPF;

    printf("\n----------- AGENDAMENTO DE ATENDIMENTO -------------\n");
    printf("\nATENÇÃO: Só é possível agendar atendimentos para o dia seguinte.\n");
    printf("Advogados disponíveis para amanhã: \n");

    // Mostra relatório de advogados disponíveis para o dia seguinte
    mostrarRelatorioAdvDis();

    // Enquanto não encontrar um advogado com a OAB informada, pede para inserir novamente
    do
    {
        printf("\nInsira o número da OAB do advogado que deseja atendimento: ");
        scanf("%i", &tempOab);
        limparBuffer();
        adv = retornarAdvogadoEspecifico(tempOab);
    } while (adv.oab == -1);

    // Inicializa a fila para o advogado, se ainda não existir
    inicializarFila(tempOab);

    // Enquanto não encontrar um estagiário plantonista com o código informado, pede para inserir novamente
    do
    {
        printf("\nInsira o código do estagiário: ");
        scanf("%i", &codEstag);
        limparBuffer();
        estag = retornarEstagiarioEspecifico(codEstag);
        if (estag.tipo != 'P')
        {
            printf("\nSomente estagiários plantonistas podem agendar atendimentos.\n");
        }
    } while (estag.tipo != 'P');

    // Preenche a estrutura de agenda
    agenda.oab = tempOab;
    agenda.codEstagiario = codEstag;

    // Verifica se o advogado possui disponibilidade para o dia seguinte
    Disponibilidade disp = obterDisponibilidadeEspecifica(tempOab, diaSemana + 1);
    if (disp.oab == -1)
    {
        printf("\nAdvogado não possui disponibilidade para o dia seguinte.\n");
        return;
    }

    // Preenche os campos da agenda
    gerarData(agenda.dtAgendada, sizeof(agenda.dtAgendada), 1);     // Gera data do dia seguinte
    strcpy(agenda.hrAgendada, disp.horaInicio);                     // Usa a hora de início da disponibilidade
    gerarData(agenda.dtInscricao, sizeof(agenda.dtInscricao), 0);   // Data de inscrição é a data atual
    gerarHoraAtual(agenda.hrInscricao, sizeof(agenda.hrInscricao)); // Hora de inscrição é a hora atual
    strcpy(agenda.areaDireito, adv.especialidade);                  // Área do direito do advogado

    printf("\nQual tipo de orientação deseja?\n");
    fgets(agenda.tipoOrientacao, sizeof(agenda.tipoOrientacao), stdin);
    agenda.tipoOrientacao[strcspn(agenda.tipoOrientacao, "\n")] = '\0';

    // Pergunta se há um prazo fatal
    do
    {
        printf("\nPrazo fatal? (S/N): ");
        tempPF = getchar();
        limparBuffer();
    } while (tempPF != 'S' && tempPF != 'N');

    if (tempPF == 'S')
    {
        agenda.prazoFatal = true;
        printf("\nInsira a data do prazo fatal: ");
        fgets(agenda.dtPrazoFatal, sizeof(agenda.dtPrazoFatal), stdin);
        agenda.dtPrazoFatal[strcspn(agenda.dtPrazoFatal, "\n")] = '\0';
    }
    else
    {
        agenda.prazoFatal = false;
    }

    printf("\nObservações: ");
    fgets(agenda.observacoes, sizeof(agenda.observacoes), stdin);
    agenda.observacoes[strcspn(agenda.observacoes, "\n")] = '\0';
    printf("\nNúmero do processo: ");
    fgets(agenda.numProcesso, sizeof(agenda.numProcesso), stdin);
    agenda.numProcesso[strcspn(agenda.numProcesso, "\n")] = '\0';

    // Enfileira o agendamento na fila do advogado
    enfileirar(agenda);

    // Abre o arquivo para gravação
    arquivoAgenda = fopen("./data/AGENDA.DAT", "a+b");
    if (arquivoAgenda == NULL)
    {
        printf("Erro ao abrir AGENDA.DAT\n");
        exit(1);
    }
    fwrite(&agenda, sizeof(Agenda), 1, arquivoAgenda);
    fclose(arquivoAgenda);

    printf("\nATENDIMENTO AGENDADO COM SUCESSO!\n");
    gerarRelatorioAgenda(&agenda);
}

Agenda* obterTodosAgendamentos(int *count)
{
    *count = 0;
    FILE *arquivoAgenda = fopen("./data/AGENDA.DAT", "r+b");
    if (arquivoAgenda == NULL)
    {
        printf("Erro ao abrir AGENDA.DAT\n");
        system("pause");
        exit(1);
    }

    fseek(arquivoAgenda, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivoAgenda);
    int numRegistros = tamanhoArquivo / sizeof(Agenda);
    rewind(arquivoAgenda);

    if (numRegistros <= 0)
    {
        printf("Nenhum registro encontrado em AGENDA.DAT\n");
        fclose(arquivoAgenda);
        return NULL;
    }

    Agenda *agendas = (Agenda *)malloc(numRegistros * sizeof(Agenda));
    if (agendas == NULL)
    {
        printf("Erro interno. (FALHA NA ALOCAÇÃO DE MEMÓRIA)\n");
        fclose(arquivoAgenda);
        system("pause");
        exit(1);
    }

    *count = fread(agendas, sizeof(Agenda), numRegistros, arquivoAgenda);

    fclose(arquivoAgenda);

    if (*count < numRegistros)
    {
        printf("Erro ao ler todos os registros da agenda.\n");
        free(agendas);
        return NULL;
    }

    ordenaAgenda(agendas, *count);

    arquivoAgenda = fopen("./data/AGENDA.DAT", "w+b");
    if (arquivoAgenda == NULL)
    {
        printf("Erro ao abrir AGENDA.DAT para escrita\n");
        free(agendas);
        system("pause");
        exit(1);
    }

    size_t registrosEscritos = fwrite(agendas, sizeof(Agenda), *count, arquivoAgenda);
    if (registrosEscritos < *count)
    {
        printf("Erro ao salvar os registros ordenados na agenda.\n");
    }

    fclose(arquivoAgenda);

    return agendas;
}

// Insertion Sort
void ordenaAgenda(Agenda *agendas, int tamanho_vetor)
{
    int anterior;
    Agenda escolhido;

    for (int i = 1; i < tamanho_vetor; i++)
    {
        escolhido = agendas[i];
        anterior = i - 1;

        while ((anterior >= 0) && (agendas[anterior].numProcesso > escolhido.numProcesso))
        {
            agendas[anterior + 1] = agendas[anterior];
            anterior = anterior - 1;
        }

        agendas[anterior + 1] = escolhido;
    }
}

// Função para gerar o relatório do agendamento
void gerarRelatorioAgenda(Agenda *agenda)
{
    printf("\nNúmero da OAB: %i", agenda->oab);
    printf("\nCódigo do estagiário: %i", agenda->codEstagiario);
    printf("\nData de inscrição: %s", agenda->dtInscricao);
    printf("\nHora de inscrição: %s", agenda->hrInscricao);
    printf("\nData agendada: %s", agenda->dtAgendada);
    printf("\nHora agendada: %s", agenda->hrAgendada);
    printf("\nÁrea do direito: %s", agenda->areaDireito);
    printf("\nTipo de orientação: %s", agenda->tipoOrientacao);
    printf("\nPrazo fatal: %s", agenda->prazoFatal ? "Sim" : "Não");
    if (agenda->prazoFatal)
    {
        printf("\nData do prazo fatal: %s", agenda->dtPrazoFatal);
    }
    printf("\nObservações: %s", agenda->observacoes);
    printf("\nNúmero do processo: %s", agenda->numProcesso);
    printf("\nFoi atendido? %s", agenda->foiatend);
    printf("\n--------------------------------------------------------\n");
}

void mostrarHistoricoAgendamentos()
{
    int count;
    Agenda* agendas = obterTodosAgendamentos(&count);

    printf("\n-----------HISTÓRICO DE AGENDAMENTOS NO SISTEMA -------------\n");

    if (count == 0 || agendas == NULL)
    {
        printf("\nNenhum agendamento cadastrado no DJ.\n");
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            gerarRelatorioAgenda(&agendas[i]);
            if (i < count - 1)
            {
                printf("\n          ¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨         ");
            }
        }
    }

    printf("\n--------------------------------------------------------\n");

    free(agendas); // desaloca a memória
}

void adicionarObservacaoAtendimento()
{
    bool encontrado = false;
    Agenda agenda;
    char numProcesso[20];

    FILE *arquivoAgenda = fopen("./data/AGENDA.DAT", "r+b");
    if (arquivoAgenda == NULL)
    {
        printf("Erro ao abrir AGENDA.DAT\n");
        system("pause");
        exit(1);
    }

    printf("\n----------- ADICIONAR OBSERVAÇÃO -------------\n");

    printf("\nQual o número do processo que você deseja adicionar observações? ");
    fgets(numProcesso, sizeof(numProcesso), stdin);
    numProcesso[strcspn(numProcesso, "\n")] = '\0'; // Remover o '\n' capturado pelo fgets

    while (fread(&agenda, sizeof(Agenda), 1, arquivoAgenda))
    {
        if (strcmp(agenda.numProcesso, numProcesso) == 0)
        {
            encontrado = true;
            gerarRelatorioAgenda(&agenda);

            printf("\nObservações (atual): %s\n", agenda.foiatend);

            printf("\nDigite as novas observações: ");
            char novasObservacoes[500];  // Buffer para novas observações
            fgets(novasObservacoes, sizeof(novasObservacoes), stdin);
            novasObservacoes[strcspn(novasObservacoes, "\n")] = '\0'; // Remover '\n'

            // Sobrescreve as observações existentes com as novas
            strncpy(agenda.foiatend, novasObservacoes, sizeof(agenda.foiatend) - 1);
            agenda.foiatend[sizeof(agenda.foiatend) - 1] = '\0'; // Garante que a string esteja terminada em '\0'

            // Voltar para o início do registro para atualizar
            fseek(arquivoAgenda, -sizeof(Agenda), SEEK_CUR);
            fwrite(&agenda, sizeof(Agenda), 1, arquivoAgenda);

            printf("\nOBSERVAÇÃO ATUALIZADA\n");
            break;
        }
    }

    if (!encontrado)
    {
        printf("\nNenhum processo encontrado com o número de processo: %s\n", numProcesso);
    }

    fclose(arquivoAgenda);
}

