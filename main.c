#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "sensores.h"
#include "alertas.h"
#include "historico.h"

// Função auxiliar para limpar a fila
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
    PilhaSincronizacoes historico; 

    inicializar_fila(&fila_pendentes);
    inicializar_pilha(&historico);
    
    int opcao, id, sub, n;
    char buffer[50];

    do {
        printf("\n=== SISTEMA CENTRAL DE DADOS ===\n");
        printf("1. Descarregar/Carregar Dados (Gera Alertas)\n");
        printf("2. Listar Sensores\n");
        printf("3. Pesquisar Codigo\n");
        printf("4. Listar por Estado ou Tipo\n");
        printf("5. Gestao de Alertas (Pendentes/Processar/Concluir)\n");
        printf("6. Historico de Sincronizacoes (Pilha)\n");
        printf("7. Guardar/Carregar Binario\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        while (getchar() != '\n'); 

        switch (opcao) {
            case 1:
                libertar_lista(lista_sensores);
                limpar_fila(&fila_pendentes);
                lista_sensores = NULL;

                int n_leituras = descarregar_dados_api("https://sensorlab.innominatum.pt/v1/sensors/export/txt", txt_ficheiro);
                n_leituras = carregar_sensores_txt(&lista_sensores, txt_ficheiro);
                
                verificar_e_registar_alertas(lista_sensores, &fila_pendentes);
                
                push_sincronizacao(&historico, n_leituras, 0);
                printf("Dados carregados e sincronizacao registada no historico!\n");
                break;
            case 2: listar_sensores(lista_sensores); break;
            case 3: 
                printf("Codigo: "); fgets(buffer, 50, stdin); buffer[strcspn(buffer, "\n")] = 0;
                pesquisar_sensor_codigo(lista_sensores, buffer); break;
            case 4:
                printf("\n--- LISTAGEM POR FILTRO ---\n");
                printf("1. Estado\n");
                printf("2. Tipo\n");
                printf("Escolha: ");
                scanf("%d", &sub); while(getchar() != '\n');
                if(sub==1) { printf("Estado: "); fgets(buffer, 50, stdin); buffer[strcspn(buffer, "\n")] = 0; listar_sensores_estado(lista_sensores, buffer); }
                else { printf("Tipo: "); fgets(buffer, 50, stdin); buffer[strcspn(buffer, "\n")] = 0; listar_sensores_tipo(lista_sensores, buffer); }
                break;
            case 5:
                printf("\n--- GESTAO DE ALERTAS ---\n");
                printf("1. Listar Pendentes\n");
                printf("2. Processar Proximo\n");
                printf("3. Concluir Alerta\n");
                printf("Escolha: ");
                scanf("%d", &sub); while(getchar() != '\n');
                if(sub==1) listar_alertas_pendentes(&fila_pendentes);
                else if(sub==2) processar_proximo_alerta(&fila_pendentes, &lista_concluidos);
                else { printf("ID: "); scanf("%d", &id); concluir_alerta(&lista_concluidos, id); }
                break;
            case 6:
                printf("\n--- HISTORICO (PILHA) ---\n");
                printf("1. Consultar Ultima\n");
                printf("2. Listar N recentes\n");
                printf("3. Remover ultima\n");
                printf("Escolha: ");
                scanf("%d", &sub); while(getchar() != '\n');
                if(sub==1) consultar_ultima_sincronizacao(&historico);
                else if(sub==2) { printf("N: "); scanf("%d", &n); listar_n_sincronizacoes(&historico, n); }
                else pop_sincronizacao(&historico);
                break;
            case 7:
                printf("\n--- FICHEIROS BINARIOS ---\n");
                printf("1. Guardar Binario\n");
                printf("2. Carregar Binario\n");
                printf("Escolha: ");
                scanf("%d", &sub); while(getchar() != '\n');
                if(sub==1) guardar_sensores_binario(lista_sensores, bin_ficheiro);
                else lista_sensores = carregar_sensores_binario(bin_ficheiro); break;
        }
    } while (opcao != 0);

    libertar_lista(lista_sensores);
    return 0;
}