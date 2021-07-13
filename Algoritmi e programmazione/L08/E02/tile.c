#include <stdio.h>
#include <stdlib.h>
#include "tile.h"

Tile *TILEread(int *T) {
    int i;
    Tile *tiles;
    FILE *f = fopen("tiles.txt", "r");

    fscanf(f, "%d ", T);
    tiles = (Tile *)malloc(*T * sizeof(Tile));
    for(i = 0; i < *T; i++) {
        tiles[i].portions[0] = TUBEread(f);
        tiles[i].portions[1] = TUBEread(f);
        tiles[i].used = 0;
    }

    return tiles;
}
Tube TUBEread(FILE *f) {
    Tube tube;

    fscanf(f, "%c %d ", &tube.color, &tube.value);

    return tube;
}
void TILEfree(Tile *tiles) {
    free(tiles);
}
void TILEprint(Tile *tiles, int T) {
    int i;
    for (i = 0; i < T; i++)
        printf("%c %d %c %d \n", tiles[i].portions[0].color, tiles[i].portions[0].value, tiles[i].portions[1].color, tiles[i].portions[1].value);
}