#include "element.h"
#include "diagonal.h"

#ifndef PROGRAM_H
#define PROGRAM_H

#define PROG_LEN 3
#define DIFFICULTY_FOR_BONUS 8

typedef struct {
    Diagonal diagonals[PROG_LEN];
    float value;
} Program;

float PROGevaluate(Program prog, Element *elems);

#endif
