
//L05E01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_TITLE_LEN = 255;

void generaPlaylist(char ***mat, int *n, int pro);
void playlist(char ***mat, char **list, int *n, int pro, int liv);

int main () {
    FILE *fin;
    char ***brani;
    int N, *n, i, j;

    if ((fin = fopen("brani.txt", "r")) == NULL) {
        printf ("Il file brani.txt non %c stato trovato.\n", 138);
        return EXIT_FAILURE;
    }
    fscanf (fin, "%d", &N);
    brani = (char ***)malloc(N * sizeof(char **));
    n = (int *)malloc(N * sizeof(int));
    for (i = 0; i < N; i++) {
        fscanf (fin, "%d", &n[i]);
        brani[i] = (char **)malloc(n[i] * sizeof(char *));
        for (j = 0; j < n[i]; j++) {
            brani[i][j] = (char*)malloc(MAX_TITLE_LEN*sizeof(char));
            fscanf (fin, "%s", brani[i][j]);
        }
    }
    fclose (fin);
    generaPlaylist(brani, n, N);
    return 0;
}

/*
fuzione wrapper che alloca il vettore contenente le canzoni di una playlist e chiama la funzione che genera le playlist
*/
void generaPlaylist (char ***mat, int *n, int pro) {
    char **list;
    list = (char **)malloc(pro * sizeof(char *));
    for (int i = 0; i < pro; i++) {
        list[i] = (char *)malloc(MAX_TITLE_LEN* sizeof(char));
    }
    playlist(mat, list, n, pro, 0);
}

/*
funzione di generazione delle playlist andando in prfondità nel grafo che le rappresenta e con un backtrack comprendo
tutte le possibili combinazioni. Arrivato al caso di selezione di un numero di canzoni per coprire la playlist la stampa
ed effettua il backtrack.

mat : matrice con tutte le canzoni disposnibili
list : vettore da stampare con le canzoni della playlist
n : numero di canzoni per amico
pro : numero di amici
liv : numero dell'amico del quale si stanno scegliendo le canzoni
*/
void playlist (char ***mat, char **list, int *n, int pro, int liv) {
    int i, j;

    if (liv == pro-1) {
        for (i = 0; i < n[liv]; i++) {
            for (j = 0; j < pro-1; j++) {
                printf("%s ", list[j]);
            }
            printf("%s\n", mat[liv][i]);
        }
        return;
    }

    for (i = 0; i < n[liv]; i++) {
        strcpy(list[liv], mat[liv][i]);
        playlist (mat, list, n, pro, liv+1);
    }
    return;
}
