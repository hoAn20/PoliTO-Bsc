/*
 * Cagnasso Pietro
 * mat 256604
 * Algoritmi e programmazione
 * a.a. 2019-2020
 * */

//L00E04

#include <stdio.h>
#include <stdlib.h>

const int MAX_R = 20;
const int MAX_C = 20;
const int MAX_STR_LEN = 20;

int min(int a, int b);
void findSubMat(int m[MAX_R][MAX_C], int r, int c, int d);

int main() {
    FILE *f;
    int nr,nc, dim, mat[MAX_R][MAX_C];
    char file_name[MAX_STR_LEN];
    int i, j, minrc;

    printf("Inserisci il nome del file da cui leggere la matrice: ");
    gets(file_name);
    if((f = fopen(file_name, "r")) == NULL) {
        printf("Non %c stato possibile recuperare il file. Riprovare.\n", 138);
        system("pause");
        return EXIT_FAILURE;
    }

    fscanf(f, "%d %d", &nr, &nc);
    for(i = 0; i < nr; i++) {
        for(j = 0; j < nc; j++) {
            fscanf(f, "%d", &mat[i][j]);
        }
    }

    printf("La matrice letta %c %dx%d, inserire la dimensione di una sottomatrice (0 o maggiore di una delle dimensioni per terminare): ", 138, nr, nc);
    scanf("%d", &dim);
    minrc = min(nr, nc);
    while(dim < 1 || dim > minrc) {
        findSubMat(mat, nr, nc, dim);

        printf("La matrice letta %c %dx%d, inserire la dimensione di una sottomatrice (0 o maggiore di una delle dimensioni per terminare): ");
        scanf("%d", &dim);
    }

    system("pause");
    return 0;
}

int min(int a, int b) {
    if(a < b) {
        return a;
    } else {
        return b;
    }
}

void findSubMat(int m[MAX_R][MAX_C], int r, int c, int d) {
    int i, j, ii, jj;
    int max = INT_MIN, sum, mat[MAX_R - 1][MAX_C - 1];

    for(i = 0; i + d < r; i++) {
        for(j = 0; j + d < c; j++) {
            sum = 0;
            for(ii = i; ii - i < d; i++) {
                for(jj = j; jj - j < d; j++) {
                    printf("%d ", m[ii][jj]);
                    sum += m[ii][jj];
                }
                printf("\n");
            }
            printf("\n");
            if(sum > max) {
                max = sum;
                for(ii = i; ii - i < d; i++) {
                    for(jj = j; jj - j < d; j++) {
                        mat[ii-i][jj-j] = m[i][j];
                    }
                }
            }
        }
    }
    printf("La sotto matrice con somma degli elementi massima %c:\n", 138);
    for(i = 0; i < d; i++){
        for(j = 0; j < d; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}
