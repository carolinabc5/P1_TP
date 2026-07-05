#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "sensores.h"

int main() {
    const char* url = "https://sensorlab.innominatum.pt/v1/sensors/export/txt";
    const char* txt_ficheiro = "resposta_api.txt";
    const char* bin_ficheiro = "dados_sensores.bin";
    
    Sensor* lista_sensores = NULL;
    int opcao;
    char buffer[50]; // Para ler inputs do teclado

    do {
        printf("\n=== SISTEMA DE MONOTORIZACAO ===\n");
        printf("1. Descarregar dados da API (Modulo 1)\n");
        printf("2. Carregar dados do ficheiro TXT para a Memoria\n");
        printf("3. Listar todos os sensores\n");
        printf("4. Pesquisar sensor por Codigo\n");
        printf("5. Listar sensores por Estado\n");
        printf("6. Listar sensores por Tipo\n");
        printf("7. Guardar dados em Ficheiro Binario\n");
        printf("8. Carregar dados de Ficheiro Binario\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        // Limpa o 'Enter' da memória para evitar bugs nos próximos inputs
        while (getchar() != '\n'); 

        switch (opcao) {
            case 1:
                if (descarregar_dados_api(url, txt_ficheiro)) {
                    printf("Download concluido com sucesso!\n");
                }
                break;
            case 2:
                // Liberta a memória da lista antiga se já existirem dados (Boa prática)
                lista_sensores = NULL; 
                carregar_sensores_txt(&lista_sensores, txt_ficheiro);
                printf("Dados em texto carregados para a Lista Ligada.\n");
                break;
            case 3:
                listar_sensores(lista_sensores);
                break;
            case 4:
                printf("Digite o codigo do sensor: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0; // Remove o 'Enter' do fim
                pesquisar_sensor_codigo(lista_sensores, buffer);
                break;
            case 5:
                printf("Digite o estado (ex: NORMAL, ALERTA): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                listar_sensores_estado(lista_sensores, buffer);
                break;
            case 6:
                printf("Digite o tipo (ex: Temperatura, Humidade): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                listar_sensores_tipo(lista_sensores, buffer);
                break;
            case 7:
                guardar_sensores_binario(lista_sensores, bin_ficheiro);
                break;
            case 8:
                lista_sensores = carregar_sensores_binario(bin_ficheiro);
                break;
            case 0:
                printf("A encerrar a aplicacao...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}