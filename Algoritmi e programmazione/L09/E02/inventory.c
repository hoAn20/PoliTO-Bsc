#include "inventory.h"

struct inventory {
    Object *inventoryArray;
    int invSize;
};

Inventory INVENTORYnew() {
    Inventory inv;

    inv = (Inventory)malloc(sizeof(*inv));

    return inv;
}
void INVENTORYfree(Inventory inv) {
    free(inv->inventoryArray);
    free(inv);
}
void INVENTORYread(Inventory inv) {
    FILE *f;
    int i, N;
    char fileName[STR_LEN+1];

    do {
        printf("Inserisci il nome del file da ci leggere l'inventario: ");
        scanf("%s", fileName);
        f = fopen(fileName, "r");
    } while(f == NULL);

    fscanf(f, "%d", &N);
    inv->inventoryArray = (Object *)malloc(N * sizeof(Object));
    inv->invSize = N;
    for (i = 0; i < N; i++)
        inv->inventoryArray[i] = OBJread(f);
}
void INVENTORYprint(Inventory inv) {
    int i;

    for (i = 0; i < inv->invSize; i++) {
        printf("%d) ", (i+1));
        OBJprint(inv->inventoryArray[i], 1);
    }
}
void INVENTORYprintObjectByIndex(Inventory inv, int index) {
    OBJprint(inv->inventoryArray[index], 0);
}
int INVENTORYgetSize(Inventory inv) {
    return inv->invSize;
}
Object INVENTORYgetObjectByIndex(Inventory inv, int index) {
    return inv->inventoryArray[index];
}