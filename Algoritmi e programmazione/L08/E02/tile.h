#ifndef TILE_H
#define TILE_H

typedef struct {
    char color;
    int value;
} Tube;

typedef struct {
    Tube portions[2];
    int used;
} Tile;

Tile *TILEread(int *T);
Tube TUBEread(FILE *f);
void TILEfree(Tile *tiles);
void TILEprint(Tile *tiles, int T);

#endif