#include <stdio.h>
#include <stdlib.h>

#ifndef PIATTO_H
#define PIATTO_H

typedef struct {
    char nome[100];
    char portata[100];
    char tipo[100];
    float costo;
} Piatto;

Piatto PIATTOread(FILE *f);
void PIATTOprint(Piatto p);

#endif
