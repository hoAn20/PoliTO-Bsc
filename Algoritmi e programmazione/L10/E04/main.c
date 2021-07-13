#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "computer.h"
#include "symtab.h"
#include "list.h"
#include "graph.h"

int *AlphaIndexOreder(Graph graph);

int main() {
    FILE *fin = fopen("grafo.txt", "r");
    int *indexOredered;
    char n1[MAX_STR_LEN], n2[MAX_STR_LEN], n3[MAX_STR_LEN];

    if (fin == NULL) {
        printf("Errore durante l' apertura del file.\n\n");
        return EXIT_FAILURE;
    }

    Graph graph = GRAPHload(fin);

    GRAPHmAdjTolAdj(graph);

    indexOredered = AlphaIndexOreder(graph);

    printf("Stampa dei vertici in ordine alfabetico e lista dei vertici insistenti su ognuno: \n\n");
    GRAPHinsistentPrint(graph, indexOredered);

    printf("\n\nDati 3 vertici verificare che siano adiacenti a coppie\n\n");
    printf("Inserisci il primo nome: ");
    scanf("%s", n1);
    printf("Inserisci il secondo nome: ");
    scanf("%s", n2);
    printf("Inserisci il terzo nome: ");
    scanf("%s", n3);
    printf("Versione con matrice di adiacenza:\n\n");
    if(GRAPHverifyCoupleAdjMatrix(graph, n1, n2, n3))
        printf("Compongono un sottografo completo.\n");
    else
        printf("Non compongono un sottografo completo.\n");
    printf("Versione con lista di adiacenza:\n\n");
    if(GRAPHverifyCoupleAdjList(graph, n1, n2, n3))
        printf("Compongono un sottografo completo.\n");
    else
        printf("Non compongono un sottografo completo.\n");

    return 0;
}

int *AlphaIndexOreder(Graph graph) {
    int *indices = malloc(GRAPHreturnSize(graph) * sizeof(int));
    int t;

    for (int i = 0; i < GRAPHreturnSize(graph); i++)
        indices[i] = i;
    for (int i = 0; i < GRAPHreturnSize(graph) - 1 ; i++) {
        for (int j = i+1; j < GRAPHreturnSize(graph); j++) {
            if (strcmp(STtoCompName(GRAPHreturnSt(graph), indices[j]), STtoCompName(GRAPHreturnSt(graph), indices[i])) < 0) {
                t = indices[j];
                indices[j] = indices[i];
                indices[i] = t;
            }
        }
    }

    return indices;
}
