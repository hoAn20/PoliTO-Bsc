#include <stdio.h>
#include <string.h>
#include "inv.h"

inv_t NEWobj(char *name, char *type, stat_t mod) {
    inv_t newInv;

    strcpy(newInv.name, name);
    strcpy(newInv.type, type);
    newInv.modifiers = mod;

    return newInv;
}
void PRINTinv(inv_t obj) {
    printf("%s %s", obj.name, obj.type);
    PRINTstat(obj.modifiers);
    printf("\n");
}