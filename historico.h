#ifndef HISTORICO_H
#define HISTORICO_H

typedef struct Sincronizacao {
    char data_hora[30];
    int num_leituras;
    int num_alertas;
    struct Sincronizacao* proximo;
} Sincronizacao;

typedef struct {
    Sincronizacao* topo;
} PilhaSincronizacoes;

void inicializar_pilha(PilhaSincronizacoes* p);
void push_sincronizacao(PilhaSincronizacoes* p, int leituras, int alertas);
void consultar_ultima_sincronizacao(PilhaSincronizacoes* p);
void listar_n_sincronizacoes(PilhaSincronizacoes* p, int n);
void pop_sincronizacao(PilhaSincronizacoes* p);

#endif