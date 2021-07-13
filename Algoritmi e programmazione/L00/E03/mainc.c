/*
 * Cagnasso Pietro
 * mat 256604
 * Algoritmi e programmazione
 * a.a. 2019-2020
 * */

//L00E03

#include <stdio.h>
#include <stdlib.h>

#define MAX_N 30

int *rotate(int v[MAX_N], int N, int m);

int main(){
    int vectLength, vect1[MAX_N], *vect2, direction, movement;
    unsigned int  nPositions;
    int i;

    do {
        printf("Inserisci il numero deli elementi che ti interessa inserire (massimo %d): ", MAX_N);
        scanf("%d", &vectLength);
    } while(vectLength < 0 || vectLength > MAX_N);

    for(i = 0; i < vectLength; i++) {
        printf("Inserisci l'elemento del vettore in posizione %d: ", i);
        scanf("%d", &vect1[i]);
    }
    system("cls");

    do {
        printf("Inserisci il numero di posizioni di cui ruotare il vettore (un eventuale numero negativo verrà convertito nel suo oppposto, 0 per interrompere): ");
        scanf("%d", &nPositions);
        if (nPositions != 0) {
            do {
                printf("Inserisci la direzione per lo spostamento (1 = ->, -1 = <-): ");
                scanf("%d", &direction);
            } while (abs(direction) != 1);
            if (nPositions > vectLength) {
                nPositions = nPositions % vectLength;
            }
            movement = nPositions * direction;

            vect2 = rotate(vect1, vectLength, movement);

            printf("Il vettore ottenuto %c: [ ", 138);
            for (i = 0; i < vectLength; i++) {
                printf("%d ", vect2[i]);
            }
            printf("]\n");
            system("pause");
            system("cls");
        }
    } while(nPositions != 0);
    system("pause");

    return 0;
}

int *rotate(int *v, int N, int m) {
    static int v2[MAX_N];
    int i, actualMoved;

    for(i = 0; i < N; i++) {
        actualMoved = i + m;
        if (actualMoved < 0) {
            actualMoved = N + actualMoved;
        } else if(actualMoved > (N - 1)) {
            actualMoved = actualMoved - N;
        }
        v2[i] = v[actualMoved];
    }

    return v2;
}
