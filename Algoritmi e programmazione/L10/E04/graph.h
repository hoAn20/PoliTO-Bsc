#include "computer.h"
#include "symtab.h"
#include "list.h"

#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph *Graph;

Graph GRAPHnew(int dim);
void GRAPHfree(Graph graph);
Graph GRAPHload(FILE *f);
void GRAPHinsertEdge(Graph graph, int id1, int id2, int weight);
void GRAPHmAdjTolAdj(Graph graph);
void GRAPHinsistentPrint(Graph graph, int *indices);
ST GRAPHreturnSt(Graph graph);
int GRAPHverifyCoupleAdjMatrix(Graph graph, char *n1, char *n2, char *n3);
int GRAPHverifyCoupleAdjList(Graph graph, char *n1, char *n2, char *n3);
int GRAPHreturnSize(Graph graph);

#endif
