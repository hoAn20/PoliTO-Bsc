#include <stdio.h>
#include <stdlib.h>

#include "piatto.h"

#ifndef MENU_H
#define MENU_H

typedef struct {
    int *scelte;
    float costo;
} Menu;

void MENUprint(Menu m, Piatto *piatti, int P);
int MENUcompare(Menu m1, Menu m2, int P);

#endif
