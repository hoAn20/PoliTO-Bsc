#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

int main() {
    int T, value;
    Tile *tiles = TILEread(&T);
    Board board = BOARDread();

    initUsedTile(board, tiles);

    board = BOARDbest(board, tiles, T, &value);
    printf("Il massimo valore ottenibile con questa board e questi tiles %c: %d\n\n", 138, value);
    BOARDprint(board);

    TILEfree(tiles);
    BOARDfree(board);
    return 0;
}