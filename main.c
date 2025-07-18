#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM 27

typedef struct Aresta {
    int destino;
    double peso;
    struct Aresta* prox;
} Aresta;

typedef struct Vertice {
    Aresta* inicio;
} Vertice;

const char* estados[TAM] = {
    "Acre", "Amapa", "Amazonas", "Para", "Rondonia", "Roraima", "Tocantins",                                        // 0-6 Norte
    "Alagoas", "Bahia", "Ceara", "Maranhao", "Paraiba", "Pernambuco", "Piaui", "Rio Grande do Norte", "Sergipe",    // 7-15 Nordeste
    "Distrito Federal", "Goias", "Mato Grosso", "Mato Grosso do Sul",                                               // 16-19 Centro-Oeste
    "Espirito Santo", "Rio de Janeiro", "Minas Gerais", "Sao Paulo",                                                // 20-23 Sudeste
    "Parana", "Rio Grande do Sul", "Santa Catarina"                                                                 // 24-26 Sul
};

int regiaoDe[TAM] = {
    0,0,0,0,0,0,0,      // Norte
    1,1,1,1,1,1,1,1,    // Nordeste
    2,2,2,2,            // Centro-Oeste
    3,3,3,3,            // Sudeste
    4,4,4               // Sul
};

const char* regioes[5] = {"Norte", "Nordeste", "Centro-Oeste", "Sudeste", "Sul"};

// Mostra tabela de estados
void exibirTabelaEstados() {
    printf("\n=== Tabela de Estados ===\n");
    for(int i = 0; i < TAM; i++) {
        printf("%2d - %s\n", i, estados[i]);
    }
    printf("=========================\n\n");
}

void adicionarAresta(Vertice grafo[], int origem, int destino, double peso) {
    Aresta* nova = (Aresta*) malloc(sizeof(Aresta));
    nova->destino = destino;
    nova->peso = peso;
    nova->prox = grafo[origem].inicio;
    grafo[origem].inicio = nova;
}

void exibirGrafo(Vertice grafo[]) {
    printf("\n=== Grafo de Trocas Monetarias ===\n");
    for(int i = 0; i < TAM; i++) {
        printf("%s troca com:\n", estados[i]);
        Aresta* temp = grafo[i].inicio;
        while(temp != NULL) {
            printf("  -> %s, Valor: R$ %.2f\n", estados[temp->destino], temp->peso);
            temp = temp->prox;
        }
        printf("\n");
    }
}

void relatorioPorRegiao(Vertice grafo[]) {
    double soma[5] = {0.0};

    for(int i = 0; i < TAM; i++) {
        int regiao = regiaoDe[i];
        Aresta* temp = grafo[i].inicio;
        while(temp != NULL) {
            soma[regiao] += temp->peso;
            temp = temp->prox;
        }
    }

    printf("\n=== Relatorio por Regiao (Total de Saidas) ===\n");
    for(int i = 0; i < 5; i++) {
        printf("%s: R$ %.2f\n", regioes[i], soma[i]);
    }
}

void relatorioPorEstado(Vertice grafo[]) {
    double saiu[TAM] = {0.0};
    double entrou[TAM] = {0.0};

    for(int i = 0; i < TAM; i++) {
        Aresta* temp = grafo[i].inicio;
        while(temp != NULL) {
            saiu[i] += temp->peso;
            entrou[temp->destino] += temp->peso;
            temp = temp->prox;
        }
    }

    printf("\n=== Relatorio por Estado ===\n");
    for(int i = 0; i < TAM; i++) {
        printf("[%2d] %-20s | Saiu: R$ %.2f | Entrou: R$ %.2f\n", i, estados[i], saiu[i], entrou[i]);
    }
}

void buscarCaminho(Vertice grafo[]) {
    int origem, destino;

    printf("\n=== Busca de Caminho entre Estados ===\n");
    printf("Digite o ID do estado de origem (0 a 26): ");
    scanf("%d", &origem);

    printf("Digite o ID do estado de destino (0 a 26): ");
    scanf("%d", &destino);

    if (origem < 0 || origem >= TAM || destino < 0 || destino >= TAM) {
        printf("ID invalido(s).\n");
        return;
    }

    bool visitado[TAM] = {false};
    int anterior[TAM];
    for (int i = 0; i < TAM; i++) anterior[i] = -1;

    int fila[TAM], ini = 0, fim = 0;
    fila[fim++] = origem;
    visitado[origem] = true;

    while (ini < fim) {
        int atual = fila[ini++];
        Aresta* temp = grafo[atual].inicio;

        while (temp != NULL) {
            int vizinho = temp->destino;
            if (!visitado[vizinho]) {
                visitado[vizinho] = true;
                anterior[vizinho] = atual;
                fila[fim++] = vizinho;
                if (vizinho == destino) break;
            }
            temp = temp->prox;
        }
    }

    if (!visitado[destino]) {
        printf("Nao ha caminho entre %s e %s.\n", estados[origem], estados[destino]);
        return;
    }

    int caminho[TAM], tam = 0;
    for (int v = destino; v != -1; v = anterior[v]) {
        caminho[tam++] = v;
    }

    printf("Caminho encontrado:\n");
    for (int i = tam - 1; i >= 0; i--) {
        printf("%s", estados[caminho[i]]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}


int main() {
    Vertice grafo[TAM];
    for(int i = 0; i < TAM; i++) grafo[i].inicio = NULL;

    int origem, destino, numTrocas;
    double valor;

    printf("Digite o numero de trocas (arestas): ");
    scanf("%d", &numTrocas);

    for(int i = 0; i < numTrocas; i++) {
        printf("\nTroca %d:\n", i + 1);
        exibirTabelaEstados();

        printf("Origem (0 a 26): ");
        scanf("%d", &origem);

        printf("Destino (0 a 26): ");
        scanf("%d", &destino);

        printf("Valor monetario: ");
        scanf("%lf", &valor);

        adicionarAresta(grafo, origem - 1, destino - 1, valor);
    }

    exibirGrafo(grafo);
    relatorioPorRegiao(grafo);
    relatorioPorEstado(grafo);
    buscarCaminho(grafo);

    return 0;
}
