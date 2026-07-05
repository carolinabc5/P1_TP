#include <stdio.h>
#include <stdlib.h>
#include "api.h"

int descarregar_dados_api(const char* url, const char* nome_ficheiro) {
    char comando[512];
    int sucesso = 0;

    // Constrói o comando de terminal: curl -s "URL" -o "ficheiro"
    // O argumento '-s' significa 'silent' para não sujar o ecrã com barras de progresso
    snprintf(comando, sizeof(comando), "curl -s \"%s\" -o \"%s\"", url, nome_ficheiro);

    printf("A comunicar com o servico remoto...\n");

    // A função system() executa o comando diretamente no terminal do Windows/Linux
    int resultado = system(comando);

    // O comando curl devolve 0 quando o download é bem sucedido
    if (resultado == 0) {
        sucesso = 1;
    } else {
        printf("Erro: Ocorreu uma falha ao executar o comando de download.\n");
    }

    return sucesso;
}