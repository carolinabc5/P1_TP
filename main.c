#include <stdio.h>
#include "api.h"

int main() {
    // Utilização obrigatória deste endpoint
    const char* url = "https://sensorlab.innominatum.pt/v1/sensors/export/txt";
    const char* nome_ficheiro = "resposta_api.txt";

    printf("A estabelecer ligacao a API...\n");

    // Chama a função que criaste no Módulo 1
    if (descarregar_dados_api(url, nome_ficheiro)) {
        printf("Sucesso! Resposta guardada em %s\n", nome_ficheiro);
    } else {
        printf("Erro ao obter os dados do serviço de monotorizacao.\n");
    }

    return 0;
}