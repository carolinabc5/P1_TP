#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sensores.h"

// Insere um novo sensor no final da Lista Ligada
void inserir_sensor(Sensor** inicio, Sensor* novo) {
    if (*inicio == NULL) {
        *inicio = novo;
    } else {
        Sensor* atual = *inicio;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }
}

// Liberta a memória da lista (corrigida)
void libertar_lista(Sensor* inicio) {
    Sensor* atual = inicio;
    while (atual != NULL) {
        Sensor* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

// Lê o ficheiro de texto e guarda na Lista Ligada[cite: 1]
int carregar_sensores_txt(Sensor** inicio, const char* nome_ficheiro) {
    // IMPORTANTE: Limpa a lista anterior antes de carregar nova informação
    if (*inicio != NULL) {
        libertar_lista(*inicio);
        *inicio = NULL;
    }

    FILE* ficheiro = fopen(nome_ficheiro, "r");
    if (ficheiro == NULL) return 0;

    char linha[512];
    int lidos = 0;

    while (fgets(linha, sizeof(linha), ficheiro)) {
        Sensor* novo = (Sensor*)malloc(sizeof(Sensor));
        if (novo == NULL) break;
        novo->proximo = NULL;

        char* token = strtok(linha, ",");
        if (token) strcpy(novo->data_hora, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(novo->codigo, token);

        token = strtok(NULL, ",");
        if (token) strcpy(novo->descricao, token);

        token = strtok(NULL, ",");
        if (token) strcpy(novo->tipo, token);

        token = strtok(NULL, ",");
        if (token) novo->valor = atof(token);

        token = strtok(NULL, ",");
        if (token) strcpy(novo->unidade, token);

        token = strtok(NULL, ",");
        if (token) strcpy(novo->estado, token);

        token = strtok(NULL, ",");
        if (token) novo->severidade = atoi(token);

        token = strtok(NULL, ",");
        if (token) strcpy(novo->tendencia, token);

        token = strtok(NULL, "\n");
        if (token) strcpy(novo->mensagem, token);

        inserir_sensor(inicio, novo);
        lidos++;
    }

    fclose(ficheiro);
    return lidos;
}

// Listar todas as leituras disponíveis[cite: 1]
void listar_sensores(Sensor* inicio) {
    if (inicio == NULL) {
        printf("A lista esta vazia.\n");
        return;
    }
    Sensor* atual = inicio;
    printf("\n--- TODAS AS LEITURAS ---\n");
    while (atual != NULL) {
        printf("Codigo: %s | Tipo: %s | Valor: %.2f %s | Estado: %s\n", 
               atual->codigo, atual->tipo, atual->valor, atual->unidade, atual->estado);
        atual = atual->proximo;
    }
}

// Pesquisar uma leitura por código[cite: 1]
void pesquisar_sensor_codigo(Sensor* inicio, const char* codigo) {
    Sensor* atual = inicio;
    int encontrado = 0;
    printf("\n--- PESQUISA POR CODIGO: %s ---\n", codigo);
    while (atual != NULL) {
        if (strcmp(atual->codigo, codigo) == 0) {
            printf("Data: %s | Desc: %s | Valor: %.2f %s | Estado: %s | Msg: %s\n",
                   atual->data_hora, atual->descricao, atual->valor, atual->unidade, atual->estado, atual->mensagem);
            encontrado = 1;
        }
        atual = atual->proximo;
    }
    if (!encontrado) printf("Nenhum sensor encontrado.\n");
}

// Listar sensores por estado[cite: 1]
void listar_sensores_estado(Sensor* inicio, const char* estado) {
    Sensor* atual = inicio;
    int contador = 0;
    printf("\n--- SENSORES COM ESTADO: %s ---\n", estado);
    while (atual != NULL) {
        if (strcmp(atual->estado, estado) == 0) {
            printf("Codigo: %s | Tipo: %s | Valor: %.2f %s\n", 
                   atual->codigo, atual->tipo, atual->valor, atual->unidade);
            contador++;
        }
        atual = atual->proximo;
    }
    if (contador == 0) printf("Nenhum sensor encontrado com esse estado.\n");
}

// Listar sensores por tipo[cite: 1]
void listar_sensores_tipo(Sensor* inicio, const char* tipo) {
    Sensor* atual = inicio;
    int contador = 0;
    printf("\n--- SENSORES DO TIPO: %s ---\n", tipo);
    while (atual != NULL) {
        if (strcmp(atual->tipo, tipo) == 0) {
            printf("Codigo: %s | Valor: %.2f %s | Estado: %s\n", 
                   atual->codigo, atual->valor, atual->unidade, atual->estado);
            contador++;
        }
        atual = atual->proximo;
    }
    if (contador == 0) printf("Nenhum sensor encontrado desse tipo.\n");
}

// Guardar binário[cite: 1]
void guardar_sensores_binario(Sensor* inicio, const char* nome_ficheiro) {
    FILE *ficheiro = fopen(nome_ficheiro, "wb");
    if (ficheiro == NULL) return;

    Sensor* atual = inicio;
    while (atual != NULL) {
        // Grava excluindo o ponteiro 'proximo'
        fwrite(atual, sizeof(Sensor) - sizeof(Sensor*), 1, ficheiro);
        atual = atual->proximo;
    }
    fclose(ficheiro);
    printf("Dados gravados em '%s'.\n", nome_ficheiro);
}

// Carregar binário[cite: 1]
Sensor* carregar_sensores_binario(const char* nome_ficheiro) {
    FILE *ficheiro = fopen(nome_ficheiro, "rb");
    if (ficheiro == NULL) return NULL;

    Sensor* inicio = NULL;
    while (1) {
        Sensor* novo = (Sensor*)malloc(sizeof(Sensor));
        if (fread(novo, sizeof(Sensor) - sizeof(Sensor*), 1, ficheiro) == 1) {
            novo->proximo = NULL; 
            inserir_sensor(&inicio, novo);
        } else {
            free(novo);
            break;
        }
    }
    fclose(ficheiro);
    printf("Dados carregados do ficheiro binario.\n");
    return inicio;
}