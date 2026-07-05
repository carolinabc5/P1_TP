#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "historico.h"

void inicializar_pilha(PilhaSincronizacoes* p) {
    p->topo = NULL;
}

void push_sincronizacao(PilhaSincronizacoes* p, int leituras, int alertas) {
    Sincronizacao* novo = (Sincronizacao*)malloc(sizeof(Sincronizacao));
    if (!novo) return;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(novo->data_hora, "%02d-%02d-%d %02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    
    novo->num_leituras = leituras;
    novo->num_alertas = alertas;
    novo->proximo = p->topo;
    p->topo = novo;
}

void consultar_ultima_sincronizacao(PilhaSincronizacoes* p) {
    if (p->topo == NULL) { printf("Historico vazio.\n"); return; }
    printf("\n--- ULTIMA SINCRONIZACAO ---\n");
    printf("Data: %s | Leituras: %d | Alertas: %d\n", p->topo->data_hora, p->topo->num_leituras, p->topo->num_alertas);
}

void listar_n_sincronizacoes(PilhaSincronizacoes* p, int n) {
    Sincronizacao* atual = p->topo;
    int count = 0;
    printf("\n--- HISTORICO (Ultimos %d) ---\n", n);
    while (atual != NULL && count < n) {
        printf("%d. Data: %s | L: %d | A: %d\n", count+1, atual->data_hora, atual->num_leituras, atual->num_alertas);
        atual = atual->proximo;
        count++;
    }
}

void pop_sincronizacao(PilhaSincronizacoes* p) {
    if (p->topo == NULL) return;
    Sincronizacao* temp = p->topo;
    p->topo = p->topo->proximo;
    free(temp);
    printf("Ultima sincronizacao removida.\n");
}