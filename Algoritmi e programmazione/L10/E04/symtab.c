#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"

struct symtab {
    Computer *array;
    int size;
    int n;
};

ST STinit(int dim) {
    ST st = malloc(sizeof(*st));

    st->array = malloc(dim * sizeof(Computer));
    st->size = dim;
    for (int i = 0; i < st->size; i++)
        st->array[i] = COMPsetNull();
    st->size = dim;
    st->n = 0;

    return st;
}
int STsearch(ST st, Computer comp) {
    for(int i = 0; i < st->size; i++) {
        if(strcmp(st->array[i].name, comp.name) == 0)
            return i;
    }
    return -1;
}
void STinsert(ST st, Computer comp, int key) {
    if (strcmp(st->array[key].name, "") == 0)
        st->array[key] = comp;
}
void STfree(ST st) {
    free(st->array);
    free(st);
}
char *STtoCompName(ST st, int index) {
    return st->array[index].name;
}
