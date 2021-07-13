
//L01E03

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_FNAME_LEN = 20;
const int MAX_COMMAND_LEN = 100;
const int MAX_MAT_DIM = 30;

void rotate(int vect[MAX_MAT_DIM], int dim, int move);

int main() {
    FILE *fileMatrix;
    int matrix[MAX_MAT_DIM][MAX_MAT_DIM], nr, nc;
    char row[MAX_COMMAND_LEN+1], fileName[MAX_FNAME_LEN+1];
    char selector[8], direction[9];
    int index, position, sel, dir, movement, vector[MAX_MAT_DIM];
    int i, j, error = 0;

    //lettura del nome del file e successiva apertura
    printf("Inserisci il nome del file da ci leggere la matrice: ");
    gets(fileName);
    if((fileMatrix = fopen(fileName, "r")) == NULL) {
        //gestione di un errore durante l'apertura del file
        printf("Non %c possibile aprire il file specificato. Riprovare.\n\n", 138);
        system("pause");
        return EXIT_FAILURE;
    }

    fscanf(fileMatrix,"%d %d", &nr, &nc);
    if(nc > MAX_MAT_DIM || nr > MAX_MAT_DIM) {
        //gestione errore nel formato del file
        printf("Nel file inserito il numero di righe o il numero di colonne sfora il massimo consentito o %c negativo.\n\n", 138);
        system("pause");
        return EXIT_FAILURE;
    }
    for(i = 0; i < nr; i++) {
        for(j = 0; j < nc; j++) {
            fscanf(fileMatrix,"%d", &matrix[i][j]);
        }
    }

    //lettura della stringa di comando
    printf("Ricorda: <selettore> <indice> <direzione> <posizioni>\n"
           "<selettore>: riga, colonna o fine\n"
           "<indice>: indice della riga su cui lavorare\n"
           "<direzione>: destra, sinistra, su, giu\n"
           "<posizioni>: numero di posti di cui effettuare la rotazione (sempre positivo)\n\n");
    printf("Inserisci la stringa di comando: ");
    gets(row);
    sscanf(row, "%s %d %s %d", selector, &index, direction, &position);

    while(strcmp(selector, "fine") != 0) {
        //gestione di errori nel formato della stringa di comando
        if(strcmp(selector, "riga") == 0 || strcmp(selector, "colonna") == 0) {
            if(strcmp(selector, "riga") == 0){
                sel = 1;
                if(strcmp(direction, "destra") == 0 || strcmp(direction, "sinistra") == 0) {
                    if(strcmp(direction, "destra") == 0) {
                        dir = -1;
                    } else {
                        dir = 1;
                    }
                } else {
                    error = 1;
                }
                if(position > nc) {
                    //in questo modo si riduce il numero di movimenti da effetturare ad un massimo di nc
                    position = position % nc;
                } else if(position < 0) {
                    error = 1;
                }
            } else {
                sel = 2;
                if(strcmp(direction, "giu") == 0 || strcmp(direction, "su") == 0) {
                    if(strcmp(direction, "giu") == 0) {
                        dir = -1;
                    } else {
                        dir = 1;
                    }
                } else {
                    error = 1;
                }
            }
            if(position > nr) {
                //in questo modo si riduce il numero di movimenti da effetturare ad un massimo di nr
                position = position % nr;
            } else if(position < 0) {
                error = 1;
            }
        } else {
            error = 1;
        }

        //nel caso non ci siano errori
        if(!error) {
            movement = dir * position;

            if(sel == 1) {
                //estraggo il vettore riga interssato e lo inivio alla funzione responsabile della rotazione per poi
                //inserirlo nuovamente
                for(i = 0; i < nc; i++) {
                    vector[i] = matrix[index][i];
                }
                rotate(vector, nc, movement);
                for(i = 0; i < nc; i++) {
                    matrix[index][i] = vector[i];
                }
            } else {
                //estraggo il vettore colonna interssato e lo inivio alla funzione responsabile della rotazione per poi
                //inserirlo nuovamente
                for(i = 0; i < nr; i++) {
                    vector[i] = matrix[i][index];
                }
                rotate(vector, nr, movement);
                for(i = 0; i < nr; i++) {
                    matrix[i][index] = vector[i];
                }
            }

            //stampa della matrice dopo la rotazione
            for(i = 0; i < nr; i ++){
                for(j = 0; j < nc; j++) {
                    printf("%d ", matrix[i][j]);
                }
                printf("\n");
            }
        } else {
            printf("Errore nella stringa di comando, correggerlo e riprovare.\n\n");
            error = 0;
        }

        system("pause");
        system("cls");

        printf("Inserisci la stringa di comando: ");
        gets(row);
        sscanf(row, "%s %d %s %d", selector, &index, direction, &position);
    }

    fclose(fileMatrix);
    return 0;
}

/*
 * La funzione prende il vettore estratto dalla matrice adatta il movimento alla dimensione della matrice (del
 * vettore) in modo da usare un solo parametro per ruotare il vettore, in questo modo però deve sfruttare un vettore
 * ausiliario, la direzione di movimento è gestita nel main con un segno + o -
 *
 * vect:    vettore estratto dalla matrice
 * dim:     lunghezza del vettore
 * move:    lunghezza del movimento di cui effettuare la rotazione
 */
void rotate(int vect[MAX_MAT_DIM], int dim, int move){
    int vectTemp[MAX_MAT_DIM];
    int i, actualMove;

    for(i = 0; i < dim; i++) {
        actualMove = i + move;
        if (actualMove < 0) {
            actualMove += dim;
        } else if(actualMove > (dim - 1)) {
            actualMove -= dim;
        }
        vectTemp[i] = vect[actualMove];
    }
    for(i = 0; i < dim; i++) {
        vect[i] = vectTemp[i];
    }

    return;
}
