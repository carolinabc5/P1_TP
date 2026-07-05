# Projeto: Sistema de Monitorização de um Centro de Dados

## Identificação
* **Unidade Curricular:** Programação I
* **Instituição:** Escola Superior de Tecnologia e Gestão (ESTG) - Instituto Politécnico de Viana do Castelo (IPVC)
* **Projeto:** Sistema de Monitorização de um Centro de Dados - Época Normal

## Introdução
Este projeto foi desenvolvido como cliente de uma API pública para a monitorização em tempo real de condições ambientais e infraestrutura técnica de um Centro de Dados. A aplicação permite a recolha, armazenamento e gestão de eventos críticos.

## Estrutura do Projeto (Módulos e Estruturas de Dados)
O sistema está dividido em quatro módulos fundamentais, utilizando estruturas de dados adequadas para cada função:

* **Módulo 1 - Comunicação (API):** Estabelece a ligação ao endpoint `https://sensorlab.innominatum.pt/v1/sensors/export/txt`.
* **Módulo 2 - Gestão de Sensores:** Armazena as leituras em memória utilizando uma **Lista Ligada**. Inclui persistência em ficheiros binários e de texto.
* **Módulo 3 - Gestão de Alertas:** Utiliza uma **Fila (Queue)** para garantir que os alertas (automáticos ou manuais) sejam processados pela ordem em que são gerados (princípio FIFO).
* **Módulo 4 - Histórico:** Utiliza uma **Pilha (Stack)** para manter o registo das sincronizações (LIFO), permitindo consultar as operações mais recentes.

## Ferramentas e Bibliotecas
* **Linguagem:** C (Standard C99/C11)
* **Compilador:** GCC
* **Ambiente de Desenvolvimento:** VS Code
* **Controlo de Versões:** Git (Repositório com commits regulares para evidenciar o progresso do desenvolvimento)
* **Bibliotecas padrão:** `stdio.h`, `stdlib.h`, `string.h`, `time.h`.

## Atividades Extra e Melhorias
Para valorização da nota final, foram implementadas as seguintes melhorias:
* **Gestão de Memória Dinâmica:** Implementação robusta de funções de libertação de memória (`libertar_lista`, `limpar_fila`) para evitar *memory leaks*.
* **Interface de Utilizador (CLI) otimizada:** Estrutura de menus simplificada para uma leitura clara (uma opção por linha).
* **Robustez de Entrada:** Utilização de `fgets` e `strcspn` para evitar erros comuns de leitura de dados.
* **Automação:** Geração automática de alertas integrados no processo de carga de dados da API.

## Compilação e Execução
Para compilar o projeto, assegura-te de ter todos os ficheiros (`.c` e `.h`) na mesma pasta e utiliza o terminal:

```bash
gcc main.c api.c sensores.c alertas.c historico.c -o centro_dados.exe
./centro_dados.exe