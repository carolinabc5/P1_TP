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

// Lê o ficheiro de texto (Módulo 1) e guarda na Lista Ligada[cite: 1]
int carregar_sensores_txt(Sensor** inicio, const char* nome_ficheiro) {
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
            printf("Data: %s | Descricao: %s | Valor: %.2f %s | Estado: %s | Mensagem: %s\n",
                   atual->data_hora, atual->descricao, atual->valor, atual->unidade, atual->estado, atual->mensagem);
            encontrado = 1;
        }
        atual = atual->proximo;
    }
    
    if (!encontrado) {
        printf("Nenhum sensor encontrado com o codigo '%s'.\n", codigo);
    }
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
    
    if (contador == 0) {
        printf("Nenhum sensor encontrado com o estado '%s'.\n", estado);
    }
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
    
    if (contador == 0) {
        printf("Nenhum sensor encontrado do tipo '%s'.\n", tipo);
    }
}

// Guardar e carregar as leituras através de um ficheiro binário[cite: 1]
void guardar_sensores_binario(Sensor* inicio, const char* nome_ficheiro) {
    FILE *ficheiro = fopen(nome_ficheiro, "wb");
    if (ficheiro == NULL) {
        printf("Erro ao criar o ficheiro binario.\n");
        return;
    }

    Sensor* atual = inicio;
    int contador = 0;
    while (atual != NULL) {
        // Grava o nó atual no ficheiro binário
        fwrite(atual, sizeof(Sensor), 1, ficheiro);
        atual = atual->proximo;
        contador++;
    }
    
    fclose(ficheiro);
    printf("Sucesso! %d sensores gravados em '%s'.\n", contador, nome_ficheiro);
}

Sensor* carregar_sensores_binario(const char* nome_ficheiro) {
    FILE *ficheiro = fopen(nome_ficheiro, "rb");
    if (ficheiro == NULL) {
        printf("Aviso: Ficheiro binario '%s' nao encontrado.\n", nome_ficheiro);
        return NULL;
    }

    Sensor* inicio = NULL;
    Sensor temp; // Variável temporária para ler os dados do ficheiro
    int contador = 0;

    // Lê os blocos de dados do ficheiro, um a um
    while (fread(&temp, sizeof(Sensor), 1, ficheiro) == 1) {
        Sensor* novo = (Sensor*)malloc(sizeof(Sensor));
        if (novo != NULL) {
            *novo = temp;          // Copia os dados do ficheiro
            novo->proximo = NULL;  // Ignora o ponteiro de memória antigo
            inserir_sensor(&inicio, novo);
            contador++;
        }
    }
    
    fclose(ficheiro);
    printf("Sucesso! %d sensores carregados de '%s'.\n", contador, nome_ficheiro);
    return inicio;
}