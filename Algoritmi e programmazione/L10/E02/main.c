#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "element.h"
#include "diagonal.h"
#include "list.h"
#include "program.h"

#define DD 7
#define DP 30

List GenerateDiagonals(Element *elems, int N);
void PrintBestProgram(List diags, Element *elems, int N);
void GenerateDiagonals_r(List diags, int *actIndexArray, Element *elems, int maxDim, int actPos, int N);
void GenerateBestProgram_r(Diagonal *diags, Program *actProgram, Program *bestProgram, Element *elems, int actPos, int N, int M, int index);

int main() {
    FILE *fin = fopen("elementi.txt", "r");
    int N;
    Element *elements;
    List diagonals;

    if (fin == NULL) {
        printf("Errore nell' apertura del file.\n\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d", &N);
    elements = malloc(N * sizeof(Element));
    for (int i = 0; i < N; i++) {
        elements[i] = ELEMread(fin);
    }

    diagonals = GenerateDiagonals(elements, N);

    PrintBestProgram(diagonals, elements, N);

    free(elements);
    fclose(fin);
    return 0;
}

List GenerateDiagonals(Element *elems, int N) {
    List diagonals = LISTnew();
    int indexArray[MAX_DIAG_LEN];

    for (int k = MAX_DIAG_LEN; k > 0; k--) {
        GenerateDiagonals_r(diagonals, indexArray, elems, k, 0, N);
    }

    return diagonals;
}
void PrintBestProgram(List diags, Element *elems, int N) {
    Program actualProgram, bestProgram;
    int M = DIAGsize(diags);
    Diagonal *diagonals = LISTtoArrayAndDelete(diags);

    bestProgram.value = 0;

    GenerateBestProgram_r(diagonals, &actualProgram, &bestProgram, elems, 0, N, M, 0);

    printf("Il miglior programma ha valore: %.2f\n\n", bestProgram.value);
    for (int i = 0; i < PROG_LEN; i++) {
        printf(" - DIAGONALE #%d -\n", (i + 1));
        DIAGprint(bestProgram.diagonals[i], elems);
    }

    free(diagonals);
}
void GenerateDiagonals_r(List diags, int *actIndexArray, Element *elems, int maxDim, int actPos, int N) {
    Diagonal diag;

    if (actPos >= maxDim) {
        diag = DIAGcompose(actIndexArray, maxDim);
        if (DIAGdifficulty(diag, elems) <= DD) {
            LISTinsertInHead(diags, diag);
        }
        return;
    }

    for (int i = 0; i < N; i++) {
        if (actPos == 0) {
            if (elems[i].first == yes) {
                actIndexArray[actPos] = i;
                GenerateDiagonals_r(diags, actIndexArray, elems, maxDim, actPos + 1, N);
            }
        } else if (actPos == maxDim - 1) {
            if (elems[i].last == yes && ELEMcompatibility(elems[actIndexArray[actPos-1]], elems[i])) {
                actIndexArray[actPos] = i;
                GenerateDiagonals_r(diags, actIndexArray, elems, maxDim, actPos + 1, N);
            }
        } else {
            if (ELEMcompatibility(elems[actIndexArray[actPos-1]], elems[i])) {
                actIndexArray[actPos] = i;
                GenerateDiagonals_r(diags, actIndexArray, elems, maxDim, actPos + 1, N);
            }
        }
    }
}
void GenerateBestProgram_r(Diagonal *diags, Program *actProgram, Program *bestProgram, Element *elems, int actPos, int N, int M, int index) {
    if (actPos >= PROG_LEN) {
        actProgram->value = PROGevaluate(*actProgram, elems);
        actProgram->difficulty = PROGdifficulty(*actProgram, elems);
        if (PROGisCorrect(*actProgram, elems) && (actProgram->value > bestProgram->value) && (actProgram->difficulty < DP)) {
            *bestProgram = *actProgram;
        }
        return;
    }

    for (int i = index; i < M; i++) {
        actProgram->diagonals[actPos] = diags[i];
        GenerateBestProgram_r(diags, actProgram, bestProgram, elems, actPos+1, N, M, i+1);
    }
}
