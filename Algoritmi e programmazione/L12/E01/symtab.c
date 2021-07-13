#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"

int STgetIndexFromName(SymTab st, char* name){
    int i;
    for (i = 0; i < st.size; ++i) {
        if(!strcmp(st.a[i].name,name))
            return i;
    }

    return -1;
}
char* STgetNameFromIndex(SymTab st, int index){
    return st.a[index].name;
}

Item ITEMSetNULL(){
    Item a;
    strcpy(a.name," ");

    return a;
}
