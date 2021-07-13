#include <stdio.h>
#include <stdlib.h>

#include "program.h"


int PROGisCorrect(Program prog, Element *elems) {
    int acr[3] = {0, 0, 0};
    int acrFront = 0;
    int acrBack = 0;
    int acrSeq = 0;

    for (int i = 0; i < PROG_LEN; i++) {
        for (int j = 0; j < prog.diagonals[i].length; j++) {
            if (elems[prog.diagonals[i].indexElement[j]].type == acr_back) {
                acr[i] = 1;
                acrBack = 1;
            }
            if (elems[prog.diagonals[i].indexElement[j]].type == acr_front) {
                acr[i] = 1;
                acrFront = 1;
            }
            if (j > 0 &&
                (elems[prog.diagonals[i].indexElement[j]].type == acr_back || elems[prog.diagonals[i].indexElement[j]].type == acr_front) &&
                (elems[prog.diagonals[i].indexElement[j - 1]].type == acr_back || elems[prog.diagonals[i].indexElement[j - 1]].type == acr_front))
            {
                acrSeq = 1;
            }
        }
        if (!acr[i])
            return 0;
    }

    return acrFront && acrBack && acrSeq;
}
float PROGevaluate(Program prog, Element *elems) {
    float value[PROG_LEN] = {0};
    float bonus = (float)1.5;

    for (int i = 0; i < PROG_LEN; i++) {
        for (int j = 0; j < prog.diagonals[i].length; j++) {
            value[i] += elems[prog.diagonals[i].indexElement[j]].value;
        }
        if (i == PROG_LEN - 1 && elems[prog.diagonals[i].length - 1].difficulty >= DIFFICULTY_FOR_BONUS)
            value[i]*=bonus;
    }

    return value[0] + value[1] + value[2];
}
int PROGdifficulty(Program prog, Element *elems) {
    int difficulty = 0;

    for (int i = 0; i < PROG_LEN; i++)
        difficulty+= DIAGdifficulty(prog.diagonals[i], elems);

    return difficulty;
}