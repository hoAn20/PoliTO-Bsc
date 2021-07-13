#ifndef EQUIPARRAY_H
#define EQUIPARRAY_H

#include "invArray.h"

#define N_EQUIP 8

typedef struct {
    int inUse;
    int equipment[N_EQUIP];
} equipArray_t;

equipArray_t *NEWequipArray();
void DELequipArray(equipArray_t *equip);

#endif
