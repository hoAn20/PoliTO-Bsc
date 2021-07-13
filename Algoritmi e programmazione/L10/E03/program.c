#include <stdio.h>
#include <stdlib.h>

#include "program.h"

float PROGevaluate(Program prog, Element *elems) {
    float value[PROG_LEN] = {0};
    float bonus = 1.5;

    for (int i = 0; i < PROG_LEN; i++) {
        for (int j = 0; j < prog.diagonals[i].length; j++) {
            value[i] += elems[prog.diagonals[i].indexElement[j]].value;
        }
    }
    if (elems[prog.diagonals[PROG_LEN - 1].indexElement[prog.diagonals[PROG_LEN - 1].length - 1]].value >= DIFFICULTY_FOR_BONUS)
        value[PROG_LEN - 1] *= bonus;

    return value[0] + value[1] + value[2];
}
