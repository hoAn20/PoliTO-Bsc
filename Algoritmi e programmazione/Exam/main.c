#include <stdio.h>
#include <stdlib.h>

//librerie ad hoc
#include "piatto.h"
#include "menu.h"
#include "bst.h"

int main (int argc, char **argv) {
    FILE *f = fopen("piatti.txt", "r");
    int N, P = atoi(argv[1]);
    Piatto *ElencoPiatti;
    BST menuBst;

    fscanf(f, "%d", &N);
    ElencoPiatti = malloc(N * sizeof(Piatto));
    for (int i = 0; i < N; i++)
        ElencoPiatti[i] = PIATTOread(f);
    fclose(f);

    menuBst = BSTmenuGenera(ElencoPiatti, N, P);

    BSTmenuPrintInOrder(menuBst, ElencoPiatti, P);

    BSTfree(menuBst);
    free(ElencoPiatti);
    return 0;
}
