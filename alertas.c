#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "alertas.h"
#include "sensores.h" // Necessário para aceder à struct Sensor

int contador_id = 1; 

void inicializar_fila(FilaAlertas* fila) {
    fila->inicio = fila->fim = NULL;
}

// Regista um alerta na fila (FIFO)
void registar_alerta(FilaAlertas* fila, const char* sensor_codigo, const char* descricao, int prioridade) {
    Alerta* novo = (Alerta*)malloc(sizeof(Alerta));
    if (!novo) return;

    novo->id = contador_id++;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(novo->data_hora, "%02d-%02d-%d %02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);

    strcpy(novo->codigo_sensor, sensor_codigo);
    strcpy(novo->descricao, descricao);
    novo->prioridade = prioridade;
    strcpy(novo->estado, "Pendente");
    strcpy(novo->data_conclusao, "-");
    novo->proximo = NULL;

    if (fila->fim == NULL) {
        fila->inicio = fila->fim = novo;
    } else {
        fila->fim->proximo = novo;
        fila->fim = novo;
    }
    printf("Alerta %d registado para o sensor %s.\n", novo->id, sensor_codigo);
}

// Verifica automaticamente os sensores e gera alertas (Requisito Mínimo 8 e 9)
void verificar_e_registar_alertas(Sensor* inicio, FilaAlertas* fila) {
    Sensor* atual = inicio;
    while (atual != NULL) {
        // Regra: estado diferente de NORMAL ou severidade > 0
        if (strcmp(atual->estado, "NORMAL") != 0 || atual->severidade > 0) {
            registar_alerta(fila, atual->codigo, atual->descricao, atual->severidade);
        }
        atual = atual->proximo;
    }
}

void processar_proximo_alerta(FilaAlertas* fila, Alerta** lista_concluidos) {
    if (fila->inicio == NULL) {
        printf("Nenhum alerta pendente para processar.\n");
        return;
    }

    Alerta* temp = fila->inicio;
    fila->inicio = fila->inicio->proximo;
    if (fila->inicio == NULL) fila->fim = NULL;

    strcpy(temp->estado, "Em Curso");
    printf("A processar alerta ID %d: %s\n", temp->id, temp->descricao);
    
    // Adiciona à lista de conclusão
    temp->proximo = *lista_concluidos;
    *lista_concluidos = temp;
}

void concluir_alerta(Alerta** lista_concluidos, int id) {
    Alerta* atual = *lista_concluidos;
    while(atual != NULL) {
        if(atual->id == id && strcmp(atual->estado, "Em Curso") == 0) {
            strcpy(atual->estado, "Concluido");
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(atual->data_conclusao, "%02d:%02d", tm.tm_hour, tm.tm_min);
            printf("Alerta %d concluido.\n", id);
            return;
        }
        atual = atual->proximo;
    }
    printf("Alerta ID %d nao encontrado ou nao esta 'Em Curso'.\n", id);
}

void listar_alertas_pendentes(FilaAlertas* fila) {
    Alerta* atual = fila->inicio;
    printf("\n--- ALERTAS PENDENTES ---\n");
    if (atual == NULL) printf("Fila vazia.\n");
    while (atual != NULL) {
        printf("ID: %d | Data: %s | Sensor: %s | Desc: %s | Pri: %d\n", 
               atual->id, atual->data_hora, atual->codigo_sensor, atual->descricao, atual->prioridade);
        atual = atual->proximo;
    }
}

void listar_alertas_concluidos(Alerta* lista_concluidos) {
    Alerta* atual = lista_concluidos;
    printf("\n--- ALERTAS CONCLUIDOS ---\n");
    while (atual != NULL) {
        if(strcmp(atual->estado, "Concluido") == 0) {
            printf("ID: %d | Data Concl.: %s | Sensor: %s\n", atual->id, atual->data_conclusao, atual->codigo_sensor);
        }
        atual = atual->proximo;
    }
}