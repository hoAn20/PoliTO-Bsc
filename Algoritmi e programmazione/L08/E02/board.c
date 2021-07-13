#include <stdio.h>
#include <stdlib.h>
#include "board.h"

void BOARDbest_R(Board b, Board *bb, Tile *t, int T, int *max_v, int k, int y);
int BOARDval(Board b, Tile *t, int T);

Board BOARDread() {
    int i;
    FILE *f = fopen("board.txt", "r");
    Board board;

    fscanf(f, "%d %d", &board.R, &board.C);
    board.boardTile = (BoardTile *)malloc((board.R * board.C) * sizeof(BoardTile));
    for (i = 0; i < (board.R * board.C); i++) {
        fscanf(f, "%d/%d ", &board.boardTile[i].tileIndex, &board.boardTile[i].rotation);
        if (board.boardTile[i].tileIndex!= -1)
            board.boardTile[i].used = 1;
        else
            board.boardTile[i].used = 0;
    }

    return board;
}
void initUsedTile(Board board, Tile *tiles) {
    int i;

    for (i = 0; i < (board.R * board.C); i++) {
        if (board.boardTile[i].tileIndex!= -1)
            tiles[board.boardTile[i].tileIndex].used = 1;
    }
}
//questa è la funzione wrapper di quella che va a generare le possibili boards, azzera il valore iniziale della board e alloca lo spazio per salvare la board migliore
Board BOARDbest(Board board, Tile *tiles, int T, int *value) {
    Board boardBest;
    boardBest.boardTile = (BoardTile *)malloc((board.R * board.C) * sizeof(BoardTile));
    boardBest.R = board.R;
    boardBest.C = board.C;
    *value  = 0;

    BOARDbest_R(board, &boardBest, tiles, T, value, 0, 0);

    return boardBest;
}
void BOARDprint(Board board) {
    int i, j;
    for (i = 0; i < board.R; i++) {
        for (j = 0; j < board.C; j++) {
            printf("%d/%d ", board.boardTile[i * board.C + j].tileIndex, board.boardTile[i * board.C + j].rotation);
        }
        printf("\n");
    }
}
void BOARDfree(Board board) {
    free(board.boardTile);
}
//questa funzione genera le effettive board andando a scandire il vettore delle tiles per porre ongi tile in ogni possibile posto
void BOARDbest_R(Board b, Board *bb, Tile *t, int T, int *max_v, int k, int y) {
    int val, i;

    if (y >= (b.R * b.C)) {
        val = BOARDval(b, t, T);
        if (val > *max_v) {
            *max_v = val;
            for (i = 0; i < b.R*b.C; i++)
                bb->boardTile[i] = b.boardTile[i];
        }
    } else {
        for (; k < T; k++) {//scandisce il  vettore delle tiles
            while(t[k].used && k<T) //se la tile e già usata va a prendere quella successiva fino a trovare una non usata
                k++;
            if (t[k].used)
                return;
            while(b.boardTile[y].used)  //idem per le posizioni nella board
                y++;

            t[k].used = 1;
            b.boardTile[y].used = 1;
            b.boardTile[y].tileIndex = k;
            b.boardTile[y].rotation = 0;
            BOARDbest_R(b, bb, t, T, max_v, 0, y+1);
            b.boardTile[y].rotation = 1;
            BOARDbest_R(b, bb, t, T, max_v, 0, y+1);
            t[k].used = 0;
            b.boardTile[y].used = 0;
        }
    }
}
//questa funzione scandisce il vettore che rappresenta la matrice prima per righe poi per colonne in modo da valutare i tubi in entrambi i casi
int BOARDval(Board b, Tile *t, int T) {
    int i, j, val = 0, tmp_v;
    char color;

    //righe
    for (i = 0; i < b.R * b.C; i+= b.C) {
        tmp_v = t[b.boardTile[i].tileIndex].portions[b.boardTile[i].rotation].value;
        color = t[b.boardTile[i].tileIndex].portions[b.boardTile[i].rotation].color;    //setta il colore da andare a cercare anche nelle altre celle della riga
        for (j = i+1; j-i < b.C; j++) {
            if (t[b.boardTile[j].tileIndex].portions[b.boardTile[j].rotation].color == color)
                tmp_v+= t[b.boardTile[j].tileIndex].portions[b.boardTile[j].rotation].value;//somma su un temporaneo solo se il tile e compatibile altrimenti esce direttamente dal cilo
            else {
                tmp_v = 0;
                break;
            }
        }
        val+= tmp_v;    //aggiorna il valore in base alla somma temporanea
    }
    //colonne
    //procede in modo analogo
    for (i = 0; i < b.C; i++) {
        tmp_v = t[b.boardTile[i].tileIndex].portions[!(b.boardTile[i].rotation)].value;
        color = t[b.boardTile[i].tileIndex].portions[!(b.boardTile[i].rotation)].color;
        for (j = i+b.C; j < b.R*b.C; j+= b.C) {
            if (t[b.boardTile[j].tileIndex].portions[!(b.boardTile[j].rotation)].color == color)
                tmp_v+= t[b.boardTile[j].tileIndex].portions[!(b.boardTile[j].rotation)].value;
            else {
                tmp_v = 0;
                break;
            }
        }
        val+= tmp_v;
    }

    return val;
}
