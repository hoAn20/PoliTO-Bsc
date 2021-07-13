#ifndef BOARD_H
#define BOARD_H

#include "tile.h"

typedef struct {
    int tileIndex;
    int rotation;
    int used;
} BoardTile;

typedef struct {
    BoardTile *boardTile;
    int R;
    int C;
} Board;

Board BOARDread();
void initUsedTile(Board board, Tile *tiles);
Board BOARDbest(Board board, Tile *tiles, int T, int *value);
void BOARDprint(Board board);
void BOARDfree(Board board);

#endif