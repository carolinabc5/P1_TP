#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "sensores.h"
#include "alertas.h"

// Função auxiliar para limpar a fila antes de recarregar
void limpar_fila(FilaAlertas* fila) {
    Alerta* atual = fila->inicio;
    while(atual != NULL) {
        Alerta* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    inicializar_fila(fila);
}

int main() {
    const char* txt_ficheiro = "resposta_api.txt";
    const char* bin_ficheiro = "dados_sensores.bin";
    
    Sensor* lista_sensores = NULL;
    FilaAlertas fila_pendentes;
    Alerta* lista_concluidos = NULL;

    inicializar_fila(&fila_pendentes);
    int opcao, id;
    char buffer[50];

    do {
        printf("\n=== SISTEMA CENTRAL DE DADOS (MODULOS 1, 2 e 3) ===\n");
        printf("1. Descarregar e Carregar Dados (Gera Alertas)\n");
        printf("2. Listar Sensores | 3. Pesquisar Codigo | 4. Listar por Estado/Tipo\n");
        printf("5. Listar Alertas Pendentes | 6. Processar Alerta | 7. Concluir Alerta\n");
        printf("8. Guardar/Carregar Binario\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        while (getchar() != '\n'); 

        switch (opcao) {
            case 1:
                // Limpeza de memória antes de novo carregamento
                libertar_lista(lista_sensores);
                limpar_fila(&fila_pendentes);
                lista_sensores = NULL;

                descarregar_dados_api("https://sensorlab.innominatum.pt/v1/sensors/export/txt", txt_ficheiro);
                carregar_sensores_txt(&lista_sensores, txt_ficheiro);
                
                // Geração Automática de Alertas
                verificar_e_registar_alertas(lista_sensores, &fila_pendentes);
                printf("Dados atualizados e alertas gerados!\n");
                break;
            case 2: listar_sensores(lista_sensores); break;
            case 3: 
                printf("Codigo: "); fgets(buffer, 50, stdin); buffer[strcspn(buffer, "\n")] = 0;
                pesquisar_sensor_codigo(lista_sensores, buffer); break;
            case 4:
                printf("1. Estado | 2. Tipo: "); int sub; scanf("%d", &sub); while(getchar() != '\n');
                if(sub==1) { printf("Estado: "); fgets(buffer, 50, stdin); buffer[strcspn(buffer, "\n")] = 0; listar_sensores_estado(lista_sensores, buffer); }
                else { printf("Tipo: "); fgets(buffer, 50, stdin); buffer[strcspn(buffer, "\n")] = 0; listar_sensores_tipo(lista_sensores, buffer); }
                break;
            case 5: listar_alertas_pendentes(&fila_pendentes); break;
            case 6: processar_proximo_alerta(&fila_pendentes, &lista_concluidos); break;
            case 7:
                printf("ID do alerta a concluir: "); scanf("%d", &id);
                concluir_alerta(&lista_concluidos, id); break;
            case 8:
                printf("1. Guardar Binario | 2. Carregar Binario: "); scanf("%d", &sub);
                if(sub==1) guardar_sensores_binario(lista_sensores, bin_ficheiro);
                else lista_sensores = carregar_sensores_binario(bin_ficheiro); break;
        }
    } while (opcao != 0);

    // Limpeza final antes de sair
    libertar_lista(lista_sensores);
    return 0;
}