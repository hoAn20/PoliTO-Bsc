
//L05E02

#include <stdio.h>
#include <stdlib.h>

int **malloc2dR(FILE *f, int r, int c);
void separa(int **mat, int nr, int nc, int *bianchi, int *neri);
void free2d(int **mat, int nr);

int main() {
    FILE *f;
    int nr, nc, **mat, *bianchi, *neri, b, n, i;

    if((f = fopen("mat.txt", "r")) == NULL) {
        printf("Si %c verificato un errore durante l'apertura del file mat.txt.\n", 138);
        return EXIT_FAILURE;
    }
    fscanf(f, "%d %d", &nr, &nc);
    mat = malloc2dR(f, nr, nc);
    fclose(f);

    b = n = (nr*nc) / 2;
    if (nr%2 != 0 && nc%2 != 0)
        b++;
    bianchi = (int*)malloc(b * sizeof(int));
    neri = (int*)malloc(n * sizeof(int));
    separa(mat, nr, nc, bianchi, neri);

    printf("Bianchi:\n");
    for(i = 0; i < b; i++)
        printf("%d ", bianchi[i]);
    printf("\n\nNeri:\n");
    for(i=0; i < n; i++)
        printf("%d ", neri[i]);

    free(bianchi);
    free(neri);
    free2d(mat, nr);
    return 0;
}


/*
funzione che permette l'allocazione dinamica di una matrice 2d dato il numero di righe r e il numero di colonne c
*/
int **malloc2dR(FILE *f, int r, int c) {
    int **m, i, j;

    m = (int **)malloc(r*sizeof(int*));
    for (i = 0; i < r; i++) {
        m[i] = (int*)malloc(c*sizeof(int));
        for (j = 0; j < c; j++) {
            fscanf(f, "%d", &m[i][j]);
        }
    }

    return m;
}

/*
funzione che divide tra le caselle bianche e nere nei sottovettori adatti e li riotrna al main in modo che possano
essere stampati.
*/
void separa(int **mat, int nr, int nc, int *bianchi, int *neri) {
    int i, j, k = 0, z = 0;

    for (i = 0; i < nr; i++){
        for (j = 0; j < nc; j++) {
            if ((i + j)%2 == 0) {
                bianchi[k++] = mat[i][j];
            } else {
                neri[z++] = mat[i][j];
            }
        }
    }
}

/*
funzione che libera la memoria occupata con malloc2d
*/
void free2d(int **mat, int nr) {
    for (int i = 0; i < nr; i ++) {
        free(mat[i]);
    }
    free(mat);
}