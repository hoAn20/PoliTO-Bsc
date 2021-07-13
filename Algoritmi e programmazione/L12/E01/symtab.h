#ifndef ST
#define ST

#define MAX_STR_LEN 30

typedef struct{
    char name[MAX_STR_LEN];
} Item;

typedef struct st{
    Item* a;
    int maxN;
    int size;
} SymTab;

int STgetIndexFromName(SymTab st, char* name);
char* STgetNameFromIndex(SymTab st, int index);
Item ITEMsetNULL();

#endif
