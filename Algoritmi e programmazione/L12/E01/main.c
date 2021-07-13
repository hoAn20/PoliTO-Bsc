#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

int main() {
    Graph G = GRAPHreadFromFile("grafo4.txt");

    GRAPHfindDag(G);

    return 0;
}
