#ifndef INV_H
#define INV_H

#include "stat.h"

#define STR_LEN 50

typedef struct {
    char name[STR_LEN+1];
    char type[STR_LEN+1];
    stat_t modifiers;
} inv_t;

inv_t NEWobj(char *name, char *type, stat_t mod);
char *GETname(inv_t obj);
char *GETtype(inv_t obj);
void PRINTinv(inv_t obj);

#endif