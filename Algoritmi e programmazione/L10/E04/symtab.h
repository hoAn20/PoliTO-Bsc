#include "computer.h"

#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct symtab *ST;

ST STinit(int dim);
int STsearch(ST st, Computer comp);
void STinsert(ST st, Computer comp, int key);
void STfree(ST st);
char *STtoCompName(ST st, int index);

#endif
