#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

typedef struct {
    int u;
    int v;
    int weight;
} Edge;
struct graph {
    int V;
    int E;
    int **mAdj;
    List *lAdj;
    ST tab;

    int lAdjExist;
};

Edge EDGEnew(int id1, int id2, int weight) {
    Edge edge;
    edge.u = id1;
    edge.v = id2;
    edge.weight = weight;

    return edge;
}
int **MATRIXinit(int r, int c, int val) {
    int **matrix = malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        matrix[i] = malloc(c * sizeof(int));
        for (int j = 0; j < c; j++)
            matrix[i][j] = val;
    }

    return matrix;
}
void InsertEdge(Graph graph, Edge edge) {
    if (graph->mAdj[edge.u][edge.v] == 0)
        graph->E++;

    graph->mAdj[edge.u][edge.v] = edge.weight;
    graph->mAdj[edge.v][edge.u] = edge.weight;
}

Graph GRAPHnew(int dim) {
    Graph graph = malloc(sizeof(*graph));
    graph->V = dim;
    graph->E = 0;
    graph->mAdj = MATRIXinit(graph->V, graph->V, 0);
    graph->lAdjExist = 0;
    graph->tab = STinit(graph->V);

    return graph;
}
void GRAPHfree(Graph graph) {
    for (int i = 0; i < graph->V; i++) {
        free(graph->mAdj[i]);
        LISTfree(graph->lAdj[i]);
    }
    free(graph->mAdj);
    free(graph->lAdj);
    STfree(graph->tab);
    free(graph);
}
Graph GRAPHload(FILE *f) {
    int v=0, id1, id2, weight;
    Computer comp1, comp2;
    Graph graph;

    while((fscanf(f, "%s %s %s %s %d", comp1.name, comp1.net, comp2.name, comp2.net, &weight)) == 5)
        v++;

    graph = GRAPHnew(v);

    rewind(f);

    v = 0;
    for (int i = 0; i < graph->V; i++){
        fscanf(f, "%s %s %s %s %d", comp1.name, comp1.net, comp2.name, comp2.net, &weight);
        if (STsearch(graph->tab, comp1) == -1) {
            STinsert(graph->tab, comp1, v);
            v++;
        }
        if (STsearch(graph->tab, comp2) == -1) {
            STinsert(graph->tab, comp2, v);
            v++;
        }
        id1 = STsearch(graph->tab, comp1);
        id2 = STsearch(graph->tab, comp2);
        GRAPHinsertEdge(graph, id1, id2, weight);
    }
    graph->V = v;

    return graph;
}
void GRAPHinsertEdge(Graph graph, int id1, int id2, int weight) {
    InsertEdge(graph, EDGEnew(id1, id2, weight));
}
void GRAPHmAdjTolAdj(Graph graph) {
    if (!graph->lAdjExist) {
        graph->lAdjExist = 1;
        graph->lAdj = malloc(graph->V * sizeof(*(graph->lAdj)));
        for (int i = 0; i < graph->V; i++) {
            graph->lAdj[i] = LISTnew();
            for (int j = 0; j < graph->V; j++) {
                if (graph->mAdj[i][j] != 0)
                    LISTinsertNode(graph->lAdj[i], j);
            }
        }
    }
}
ST GRAPHreturnSt(Graph graph) {
    return graph->tab;
}
void GRAPHinsistentPrint(Graph graph, int *indices) {
    for (int i = 0; i < graph->V; i++) {
        printf("%d) %s:\t ", (i+1), STtoCompName(graph->tab, indices[i]));
        for (int j = 0; j < graph->V; j++) {
            if (i!= j && graph->mAdj[indices[j]][indices[i]] != 0) {
                printf("%s ", STtoCompName(graph->tab, indices[j]));
            }
        }
        printf("\n");
    }
}
int GRAPHverifyCoupleAdjMatrix(Graph graph, char *n1, char *n2, char *n3) {
    int id1 = STsearch(graph->tab, COMPnew(n1, " ")), id2 = STsearch(graph->tab, COMPnew(n2, " ")), id3 = STsearch(graph->tab, COMPnew(n3, " "));

    if (graph->mAdj[id1][id2]!= 0 && graph->mAdj[id1][id3]!= 0 && graph->mAdj[id3][id2]!= 0)
        return 1;
    else
        return 0;
}
int GRAPHverifyCoupleAdjList(Graph graph, char *n1, char *n2, char *n3) {
    if (graph->lAdjExist) {
        int id1 = STsearch(graph->tab, COMPnew(n1, " ")), id2 = STsearch(graph->tab, COMPnew(n2, " ")), id3 = STsearch(graph->tab, COMPnew(n3, " "));

        if (LISTpresenceCheck(graph->lAdj[id1], id2) && LISTpresenceCheck(graph->lAdj[id1], id3) && LISTpresenceCheck(graph->lAdj[id3], id2))
            return 1;
    }
    return 0;
}
int GRAPHreturnSize(Graph graph) {
    return graph->V;
}
