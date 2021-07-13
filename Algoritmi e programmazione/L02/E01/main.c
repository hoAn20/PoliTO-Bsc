
//L02E01

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int x;
    int y;
    int height;
    int width;
    int area;
} areas;

const int MAX_MAT_DIM = 50;
const int MAX_FILENAME_LEN = 20;

void findMaxs(areas mD[3], int m[MAX_MAT_DIM][MAX_MAT_DIM], int ch[MAX_MAT_DIM][MAX_MAT_DIM], int r, int c);

int main() {
    FILE *f;
    int matrix[MAX_MAT_DIM][MAX_MAT_DIM], checked[MAX_MAT_DIM][MAX_MAT_DIM];
    areas maxDims[3]; //0 altezza, 1 larghezza, 2 area
    char fileName[MAX_FILENAME_LEN];
    int nr, nc, i, j, height, width, area, ii, jj;

    //inizializzazione a 0 dei valori massimi
    for(i = 0; i < 3; i++){
        maxDims[i].width = maxDims[i].height = maxDims[i].area = 0;
    }

    printf("Insrisci il nome del file da cui leggere la matrice: ");
    gets(fileName);

    if((f = fopen(fileName, "r")) == NULL) {
        //gestione di un errore durante l'apertura del file indicato
        printf("Non %c stato possibile aprire il file specificato, riprovare.\n", 138);
        system("pause");
        return EXIT_FAILURE;
    }

    //lettura dimensioni effettive della matrice
    fscanf(f, "%d %d", &nr, &nc);
    if (nr > MAX_MAT_DIM || nc > MAX_MAT_DIM || nr < 0 || nc < 0) {
        //gestione errore nelle dimensioni effettive della matrice
        printf("Le dimensioni effettive non sono compatibili con el dimensioni massime della matrice.");
        system("pause");
        return EXIT_FAILURE;
    }
    //lettura della matrice
    for(i = 0; i < nr; i++) {
        for(j = 0; j < nc; j++) {
            fscanf(f, "%d", &matrix[i][j]);
            checked[i][j] = 0;
        }
    }
    fclose(f);

    findMaxs(maxDims, matrix, checked, nr, nc);

    printf("Massima altezza: <%d, %d> w=%d h=%d a=%d\n", maxDims[0].x, maxDims[0].y, maxDims[0].width, maxDims[0].height, maxDims[0].area);
    printf("Massima larghezza: <%d, %d> w=%d h=%d a=%d\n", maxDims[1].x, maxDims[1].y, maxDims[1].width, maxDims[1].height, maxDims[1].area);
    printf("Massima area: <%d, %d> w=%d h=%d a=%d\n", maxDims[2].x, maxDims[2].y, maxDims[2].width, maxDims[2].height, maxDims[2].area);

    system("pause");

    return 0;
}

/*
 * La funzione scandisce la matrice in cerca dell'inizio di un'area, una volta trovata ne controlla le dimensioni per
 * poi confrontarle con le dimensioni massime già registrate, nel caso abbia altezza, base o area maggiore della regione
 * con questa caratteristica salvata in mD[i] allora va a salvare la sua posizione di inizio come dato di massimo della
 * caratteristica in questione
 *
 * mD[]:    vettore di struct con le massime dimensioni
 * m[][]:   matrice da leggere in cerca delle aree di
 * ch[][]:  matrice in cui si salvano le aree già riconosciute come aree di 1
 * r:       nuero effettivo di righe
 * c:       numero effettivo di colonne
 *
 * return:  -
 */
void findMaxs(areas mD[3], int m[MAX_MAT_DIM][MAX_MAT_DIM], int ch[MAX_MAT_DIM][MAX_MAT_DIM], int r, int c) {
    int i, ii, j, jj, height, width, area;

    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            if(m[i][j] == 1 && !ch[i][j]) {
                height = 0;
                width = 0;
                do {
                    ch[i][j + width] = 1;
                    width++;
                } while(m[i][j + width] && (j + width) < c);
                do {
                    ch[i][j + height] = 1;
                    height++;
                } while(m[i + height][j] && (i + height) < r);
                area = height * width;
                for(ii = i + 1; (ii - i) < height; ii++) {
                    for(jj = j + 1; (jj - j) < width; jj++) {
                        ch[ii][jj] = 1;
                    }
                }
                if(height > mD[0].height) {
                    mD[0].x = i;
                    mD[0].y = j;
                    mD[0].height = height;
                    mD[0].width = width;
                    mD[0].area = area;
                }
                if(width > mD[1].width) {
                    mD[1].x = i;
                    mD[1].y = j;
                    mD[1].height = height;
                    mD[1].width = width;
                    mD[1].area = area;
                }
                if(area > mD[2].area) {
                    mD[2].x = i;
                    mD[2].y = j;
                    mD[2].height = height;
                    mD[2].width = width;
                    mD[2].area = area;
                }
            }
        }
    }
}
