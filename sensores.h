#ifndef SENSORES_H
#define SENSORES_H

// Estrutura do nó da Lista Ligada para os Sensores
typedef struct Sensor {
    char data_hora[30];
    char codigo[20];
    char descricao[100];
    char tipo[50];
    float valor;
    char unidade[20];
    char estado[20];
    int severidade;
    char tendencia[20];
    char mensagem[100];
    struct Sensor* proximo; // Ponteiro que liga ao próximo elemento
} Sensor;

// Assinaturas das funções exigidas pelo Módulo 2
void inserir_sensor(Sensor** inicio, Sensor* novo);
int carregar_sensores_txt(Sensor** inicio, const char* nome_ficheiro);
void listar_sensores(Sensor* inicio);
void pesquisar_sensor_codigo(Sensor* inicio, const char* codigo);
void listar_sensores_estado(Sensor* inicio, const char* estado);
void listar_sensores_tipo(Sensor* inicio, const char* tipo);
void guardar_sensores_binario(Sensor* inicio, const char* nome_ficheiro);
Sensor* carregar_sensores_binario(const char* nome_ficheiro);

#endif