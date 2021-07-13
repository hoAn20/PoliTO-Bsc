
//L04E02

#include <stdio.h>
#include <stdlib.h>

const int MAX_DIM = 20;

int majority (int *v, int N);

int main() {
    int qN, i, v[MAX_DIM], M;

    printf ("Quanti elementi ha il vettore il cu trovare il maggioritario?(massimo %d): ", MAX_DIM);
    scanf ("%d", &qN);
    for (i = 0; i < qN; i++) {
        printf ("Inserisci il numero %d-esimo: ", (i+1));
        scanf ("%d", &v[i]);
    }

    M = majority (v, qN);
    if (M >= 0) {
        printf ("L'elemento magigoritario %c: %d", 138, M);
        scanf ("%d", M);
    } else {
        printf("Non esiste elemento maggioritario.");
    }

    return 0;
}

/*
La funzione majority serve a cercare il maggioritario tra gli elementi di un vettore e lo fa procedendo ricorsivamente dividendo a metà il vettore
fino ad arrivare al caso particolare in cui essendo di lunghezza unitaria il maggioritario è l'unico elemento del vettore. In seguito controlla se
tra i due maggioritari dei sottovettori uno è maggioritario anche nel vettore che li aveva originati e così a salire fino al vettore originale.

v : vettore in cui cercare il maggioritario
N : numero degli elementi nel vettore
*/
int majority (int *v, int N) {
    int q = N/2, m1, m2, i, cnt1 = 0, cnt2 = 0;

    if (N == 1)
        return *v;

    m1 = majority (v, q);
    m2 = majority ((v+q), q);

    for (i = 0; i < N; i++) {
        if (*(v+i) == m1)
            cnt1++;
        if (*(v+i) == m2)
            cnt2++;
    }

    if (cnt1 > q)
        return m1;
    if (cnt1 > q)
        return m2;
    return -1;
}