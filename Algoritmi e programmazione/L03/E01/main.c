
//L03E01

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int x;
    int y;
    int height;
    int width;
    int area;
} coordinates;

const int MAX_MAT_DIM = 50;
const int MAX_FILENAME_LEN = 20;

int readMatrix(int mat[MAX_MAT_DIM][MAX_MAT_DIM], int *rows, int *cols);
int recognizeRegion(int mat[MAX_MAT_DIM][MAX_MAT_DIM], int rs, int cs, int r, int c, int *b, int *h);
void foundCMajor(coordinates *MDim, int x, int y, int b, int h, int a, int id);

int main() {
    int matrix[MAX_MAT_DIM][MAX_MAT_DIM], used[MAX_MAT_DIM][MAX_MAT_DIM];
    int nCol, nRow, i, j, base, height;
    coordinates maxDims[3]; //0 max base, 1 max altezza, 2 max area

    for (i = 0; i < MAX_MAT_DIM; i++) {
        for(j = 0; j < MAX_MAT_DIM; j++) {
            used[i][j] = 0;
        }
    }
    for (i = 0; i < 3; i++){
        maxDims[i].width = maxDims[i].height = maxDims[i].area = 0;
    }

    if (!readMatrix(matrix, &nRow, &nCol)) {
        //gestione di errori durante l'apertura del file
        printf("Non sono riuscito ad aprire il file specificato. Riprovare.\n");
        system("pause");
        return EXIT_FAILURE;
    } else {
        printf("Il numero effettivo di righe %c %d, il numero effettivo di colonne %c %d.\n\n", 138, nRow, 138, nCol);
    }

    for(i = 0; i < nRow; i++) {
        for(j = 0; j < nCol; j++) {
            if(recognizeRegion(matrix, nRow, nCol, i, j, &base, &height)) {
                //nal caso (i, j) sia un vertice superiore sinistro di un'area
                if (base > maxDims[0].width) {
                    foundCMajor(maxDims, i, j, base, height, base*height, 0);
                }
                if (height > maxDims[1].height) {
                    foundCMajor(maxDims, i, j, base, height, base*height, 1);
                }
                if (base*height > maxDims[2].area) {
                    foundCMajor(maxDims, i, j, base, height, base*height, 2);
                }
                printf("Nuova regione trovata in (%d, %d): w=%d, h=%d, a=%d\n", i, j, base, height, base*height);
            }
        }
    }

    printf("\nMassima larghezza: <%d, %d> w=%d h=%d a=%d\n", maxDims[0].x, maxDims[0].y, maxDims[0].width, maxDims[0].height, maxDims[0].area);
    printf("Massima altezza: <%d, %d> w=%d h=%d a=%d\n", maxDims[1].x, maxDims[1].y, maxDims[1].width, maxDims[1].height, maxDims[1].area);
    printf("Massima area: <%d, %d> w=%d h=%d a=%d\n", maxDims[2].x, maxDims[2].y, maxDims[2].width, maxDims[2].height, maxDims[2].area);


    system("pause");

    return 0;

}

/*
 * La funzione legge riga per riga il file con la matrice e va ad estrapolarne il i numeri delle casele da inserire
 * nella matrice
 *
 * mat: matrice in cui salvare i dati
 * rows:numero di righe della matrice
 * cols:numero di colonne della matrice
 *
 * return:  1 se legge correttamente la matrice, altrimenti 0 se ci sono dei problemi durante l'apertura del file
 */
int readMatrix(int mat[MAX_MAT_DIM][MAX_MAT_DIM], int *rows, int *cols) {
    FILE *fp;
    char fileName[MAX_FILENAME_LEN], row[MAX_MAT_DIM], number[2], c;
    int i = 0, j;

    printf ("Inserisci il nome del file da cui leggere la matrice: ");
    gets (fileName);
    if ((fp = fopen(fileName, "r")) == NULL) {
        return 0;
    }

    number[1] = '\0';
    i = 0;
    while (!feof(fp)) {
        fgets(row, MAX_MAT_DIM, fp);
        j = 0;
        do {
            number[0] = row[j];
            mat[i][j / 2] = atoi(number);
            c = row[j + 1];
            j += 2;
        } while(c != '\n' && c != '\0');
        i++;
    }

    *cols = j/2;
    *rows = i;

    return 1;
}

/*
 * La funzione controlla che la casella selezionata sia una possibile casella di inizio are e nel caso va a valutarne
 * le dimensioni (non l'area che tanto considerando il file corretto è data da basexaltezza)
 *
 * mat: matrice in cui cercare le aree
 * rs:  numero effettivo di righe
 * rc:  numero effettivo di colonne
 * r:   ascissa della casella da controllare
 * c:   ordinata della casella da controllare
 * b:   base della regione
 * h:   altezza della regione
 *
 * return:  0 se non è un'area, 1 se è un'area
 */
int recognizeRegion(int mat[MAX_MAT_DIM][MAX_MAT_DIM], int rs, int cs, int r, int c, int *b, int *h) {
    int i, j, base = 0, height = 0;
    if ((r == 0 && c ==0) || (r == 0 && mat[r][c-1] == 0) || (c == 0 && mat[r-1][c] == 0) || (mat[r][c-1] == 0 && mat[r-1][c] == 0)) {
        for (i = r; mat[i][c] == 1 && i < rs; i++) {
            height++;
        }
        for (j = c; mat[r][j] == 1 && j < cs; j++) {
            base++;
        }
        if (base == 0 || height == 0) {
            return 0;
        }
        *b = base;
        *h = height;
        return 1;
    } else {
        return 0;
    }
}

/*
 * La funzione va a settare i dati di una regione come massimi per il tipo di regione trovato all'interno del vettore
 * con i dati massimi
 *
 * MDim:    vettore cone le massime dimensioni
 * x:       coordinata x di inizio
 * y:       coordinata y di inizio
 * b:       base dell'area
 * h:       altezza dell'area
 * a:       area dell'area
 * id:      identficativo di massima base, altezza o area
 *
 * return:  -
 */
void foundCMajor(coordinates *MDim, int x, int y, int b, int h, int a, int id) {
    MDim[id].x = x;
    MDim[id].y = y;
    MDim[id].width = b;
    MDim[id].height = h;
    MDim[id].area = a;

    return;
}