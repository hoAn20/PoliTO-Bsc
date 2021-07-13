#include <stdio.h>
#include "inventory.h"

void inventory_print(inventoryTab_t TABin, FILE *f) {
    for (int i = 0; i < TABin.inventoryLength; i++)
        fprintf(f, "%d) %s %s %d %d %d %d %d %d\n", i+1, TABin.inventory[i].name, TABin.inventory[i].type, TABin.inventory[i].modifier.stat[hp], TABin.inventory[i].modifier.stat[mp], TABin.inventory[i].modifier.stat[atk], TABin.inventory[i].modifier.stat[def], TABin.inventory[i].modifier.stat[mag], TABin.inventory[i].modifier.stat[spr]);
}
object_t READobject(FILE *f) {
    object_t new;

    if (f == stdin) printf("Inserisci il nome del nuovo oggetto: ");
    fscanf(f, "%s", new.name);
    if (f == stdin) printf("Inserisci il tipo del nuovo oggetto: ");
    fscanf(f, "%s", new.type);
    if (f == stdin) printf("Inserisci il modificatore di hp di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[hp]);
    if (f == stdin) printf("Inserisci il modificatore di mp di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[mp]);
    if (f == stdin) printf("Inserisci il modificatore di atk di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[atk]);
    if (f == stdin) printf("Inserisci il modificatore di def di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[def]);
    if (f == stdin) printf("Inserisci il modificatore di mag di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[mag]);
    if (f == stdin) printf("Inserisci il modificatore di spr di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[spr]);

    return new;
}