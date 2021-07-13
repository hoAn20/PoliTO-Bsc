#include <stdio.h>
#include <stdlib.h>

#include "diagonal.h"

Diagonal DIAGcompose(int *indexArray, int len) {
    Diagonal diag;

    diag.length = len;

    for (int i = 0; i < diag.length; i++)
        diag.indexElement[i] = indexArray[i];

    return diag;
}
int DIAGdifficulty(Diagonal diag, Element *elems) {
    int difficulty = 0;

    for (int i = 0; i < diag.length; i++)
        difficulty+= elems[diag.indexElement[i]].difficulty;

    return difficulty;
}
void DIAGprint(Diagonal diag, Element *elems) {
    for (int i = 0; i < diag.length; i++) {
        printf("%s ", elems[diag.indexElement[i]].name);
    }
    printf("\n");
}