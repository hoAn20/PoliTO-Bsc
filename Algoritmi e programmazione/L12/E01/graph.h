#include "symtab.h"

#ifndef E01_GRAPH_H
#define E01_GRAPH_H

typedef struct graph* Graph;

Graph GRAPHreadFromFile(char* fileName);
void GRAPHfindDag(Graph g);

#endif
