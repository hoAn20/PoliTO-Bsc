#include <stdio.h>
#include <stdlib.h>

#define NTIPI 4

int ****malloc4d(int d1, int d2, int d3, int d4);
void free4D(int ****mat, int d1, int d2, int d3);
int vectMax(int *v, int dim);
int lunghezzaCollana(int z, int r, int t, int s);
int fZ(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s);
int fR(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s);
int fT(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s);
int fS(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s);

int main() {
    int d, z, r, s, t;
    FILE *f = fopen("hard_test_set.txt", "r");

    if (f == NULL) {
        printf("Errore nell'apertura del file.\n\n");
        return EXIT_FAILURE;
    }

    fscanf(f, "%d", &d);
    for (int i = 0; i < d; i++) {
        fscanf(f, "%d %d %d %d", &z, &r, &t, &s);
        printf("Collana %d: %d\n", (i+1), lunghezzaCollana(z, r ,t ,s));
    }

    return 0;
}

int ****malloc4d(int d1, int d2, int d3, int d4) {
    int ****mat;
    mat = (int ****)malloc(d1 * sizeof(int ***));
    for (int i = 0; i < d1; i++) {
        mat[i] = (int ***)malloc(d2 * sizeof(int **));
        for (int j = 0; j < d2; j++) {
            mat[i][j] = (int **)malloc(d3 * sizeof(int *));
            for (int k = 0; k < d3; k++) {
                mat[i][j][k] = (int *)malloc(d4 * sizeof(int));
                for (int w = 0; w < d4; w++)
                    mat[i][j][k][w] = -1;
            }
        }
    }
    return mat;
}
void free4D(int ****mat, int d1, int d2, int d3) {
    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++) {
            for (int k = 0; k < d3; k++) {
                free(mat[i][j][k]);
            }
            free(mat[i][j]);
        }
        free(mat[i]);
    }
    free(mat);
}
int vectMax(int *v, int dim) {
    int max = INT_MIN;

    for (int i = 0; i < dim; i++) {
        if (v[i] > max)
            max = v[i];
    }

    return max;
}
int lunghezzaCollana(int z, int r, int t, int s) {
    int ****Z, ****R, ****T, ****S;
    int len[NTIPI];

    Z = malloc4d(z+1, r+1, t+1, s+1);
    R = malloc4d(z+1, r+1, t+1, s+1);
    T = malloc4d(z+1, r+1, t+1, s+1);
    S = malloc4d(z+1, r+1, t+1, s+1);

    if (z > 0)
        len[0] = fZ(Z, R, T, S, z, r, t, s);
    if (r > 0)
        len[1] = fR(Z, R, T, S, z, r, t, s);
    if (t > 0)
        len[2] = fT(Z, R, T, S, z, r, t, s);
    if (s > 0)
        len[3] = fS(Z, R, T, S, z, r, t, s);

    free4D(Z, z+1, r+1, t+1);
    free4D(R, z+1, r+1, t+1);
    free4D(T, z+1, r+1, t+1);
    free4D(S, z+1, r+1, t+1);

    return vectMax(len, NTIPI);
}
int fZ(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s) {
    int lun[2] = {0, 0};

    if (Z[z][r][t][s] != -1)
        return Z[z][r][t][s];

    if (z == 0)
        return 0;

    lun[0] = fZ(Z, R, S, T, z-1, r, t, s);
    lun[1] = fR(Z, R, S, T, z-1, r, t, s);

    Z[z][r][t][s] = vectMax(lun, 2) + 1;

    return Z[z][r][t][s];
}
int fR(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s) {
    int lun[2] = {0, 0};

    if (R[z][r][t][s] != -1)
        return R[z][r][t][s];

    if (r == 0)
        return 0;

    lun[0] = fS(Z, R, S, T, z, r-1, t, s);
    lun[1] = fT(Z, R, S, T, z, r-1, t, s);

    R[z][r][t][s] = vectMax(lun, 2) + 1;

    return R[z][r][t][s];
}
int fT(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s) {
    int lun[2] = {0, 0};

    if (T[z][r][t][s] != -1)
        return T[z][r][t][s];

    if (t == 0)
        return 0;

    lun[0] = fZ(Z, R, S, T, z, r, t-1, s);
    lun[1] = fR(Z, R, S, T, z, r, t-1, s);

    T[z][r][t][s] = vectMax(lun, 2) + 1;

    return T[z][r][t][s];
}
int fS(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s) {
    int lun[2] = {0, 0};

    if (S[z][r][t][s] != -1)
        return S[z][r][t][s];

    if (s == 0)
        return 0;

    lun[0] = fS(Z, R, S, T, z, r, t, s-1);
    lun[1] = fT(Z, R, S, T, z, r, t, s-1);

    S[z][r][t][s] = vectMax(lun, 2) + 1;

    return S[z][r][t][s];
}