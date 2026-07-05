#ifndef ALERTAS_H
#define ALERTAS_H

typedef struct Alerta {
    int id;
    char data_hora[20];
    char codigo_sensor[20];
    char descricao[100];
    int prioridade;
    char estado[20]; // "Pendente", "Em Curso", "Concluido"
    char data_conclusao[20];
    struct Alerta* proximo;
} Alerta;

typedef struct {
    Alerta* inicio; // Fila: quem está à espera
    Alerta* fim;    // Fila: quem chegou por último
} FilaAlertas;

// Funções para a Fila
void inicializar_fila(FilaAlertas* fila);
void registar_alerta(FilaAlertas* fila, const char* sensor_codigo, const char* descricao, int prioridade);
void processar_proximo_alerta(FilaAlertas* fila, Alerta** lista_concluidos);
void concluir_alerta(Alerta** lista_concluidos, int id);
void listar_alertas_pendentes(FilaAlertas* fila);
void listar_alertas_concluidos(Alerta* lista_concluidos);

#endif