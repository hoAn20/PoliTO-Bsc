#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invArray.h"

invArray_t *NEWinvArray(int N, int actDim, int maxDim) {
    invArray_t *newInvArray;

    newInvArray->inventory = malloc(N * sizeof(inv_t));
    newInvArray->invLen = actDim;
    newInvArray->invMaxLen = maxDim;

    return newInvArray;
}
int FINDindexByName(invArray_t invArray, char *searched) {
    int i;
    for (i = 0; i < invArray.invLen; i++) {
        if (strcmp(invArray.inventory[i].name, searched) == 0)
            return i;
    }
    return -1;
}
void DELinvArray(invArray_t *invArray) {
    free(invArray->inventory);
    free(invArray);
}
void PRINTinvArray(invArray_t invArray) {
    for (int i = 0; i < invArray.invLen; i++) {
        PRINTinv(invArray.inventory[i]);
    }
}