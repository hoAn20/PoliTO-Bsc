#ifndef INVENTORY_H
#define INVENTORY_H

#define STR_LEN 50
#define N_STATS 6

enum {hp, mp, atk, def, mag, spr};

typedef struct {
    int stat[N_STATS];
} stat_t;
typedef struct {
    char name[STR_LEN + 1];
    char type[STR_LEN + 1];
    stat_t modifier;
} object_t;
typedef struct {
    object_t *inventory;
    int inventoryLength;
    int maxInventroyLength;
} inventoryTab_t;

object_t READobject(FILE *f);
void inventory_print(inventoryTab_t TABin, FILE *f);

#endif
