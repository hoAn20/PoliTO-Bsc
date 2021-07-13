#ifndef INVARRAY_H
#define INVARRAY_H

#include "inv.h"

typedef struct {
    inv_t *inventory;
    int invLen;
    int invMaxLen;
} invArray_t;

invArray_t *NEWinvArray(int N, int actDim, int maxDim);
int FINDindexByName(invArray_t invArray, char *searched);
void DELinvArray(invArray_t *invArray);
void PRINTinvArray(invArray_t invArray);

#endif