#include "element.h"

#ifndef DIAGONAL_H
#define DIAGONAL_H

#define MAX_DIAG_LEN 5

typedef struct {
    int indexElement[MAX_DIAG_LEN];
    int length;
} Diagonal;

Diagonal DIAGcompose(int *indexArray, int len);
int DIAGdifficulty(Diagonal diag, Element *elems);
void DIAGprint(Diagonal diag, Element *elems);

#endif