#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "element.h"
#include "diagonal.h"
#include "program.h"

#define DD 11
#define DP 33

void ElemOrder(Element *elems, int N);
int min(int a, int b);

int main() {
    FILE *fin = fopen("elementi.txt", "r");
    int N, maxdd, diff = 0, len = 0, index[5], i = 0;
    Element *elements;
    Program prog;

    if (fin == NULL) {
        printf("Errore nell' apertura del file.\n\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d", &N);
    elements = malloc(N * sizeof(Element));
    for (int i = 0; i < N; i++) {
        elements[i] = ELEMread(fin);
    }

    ElemOrder(elements, N);     //ordino per difficoltá

    // 1 è la migliore con un acrobatico avanti e uno indietro nella stessa purché resti in min{DD, DP/3}
    maxdd = min(DD, (int)(DP/3));
    while(elements[i].difficulty > (int)(maxdd/3))
        i++;
    while(elements[i].type != acr_front && i < N)
        i++;
    index[len] = i;
    len++;
    i = 0;
    while(elements[i].difficulty > (int)(maxdd/3) && i < N)
        i++;
    while((elements[i].type != transition || !ELEMcompatibility(elements[index[0]], elements[i]))  && i < N)
        i++;
    diff+= elements[i].difficulty;
    index[len] = i;
    len++;
    i = 0;
    while(elements[i].difficulty > ((int)(maxdd/3) + (DD - elements[index[0]].difficulty - elements[index[1]].difficulty))  && i < N)
        i++;
    while(elements[i].type != acr_back)
        i++;
    diff+= elements[i].difficulty;
    index[len] = i;
    len++;
    while(len < 5) {
        i = 0;
        while(elements[i].difficulty >= (DD - diff) && i < N)
            i++;
        if(i >= N)
            break;
        while(!ELEMcompatibility(elements[index[len]], elements[i]) && i < N)
            i++;
        if(i >= N)
            break;
        index[len] = i;
        len++;
        diff+= elements[i].difficulty;
    }
    prog.diagonals[0] = DIAGcompose(index, len);
    len = i = 0;

    // 2 è la diagonale con due acrobatici in sequenza se non l'ha giá coperta la 1
        //altrimenti quella a massimo punteggio possibile nel limite di min{[DD + (DP/3 - dd(1))], DD}
    maxdd = (int)(DD/2);
    diff = 0;
    while((elements[i].type != acr_front || elements[i].difficulty >= maxdd)  && i < N)
        i++;
    index[len] = i;
    diff+= elements[i].difficulty;
    len++;
    while((elements[i].type != acr_front || elements[i].difficulty >= maxdd)  && i < N)
        i++;
    index[len] = i;
    diff+= elements[i].difficulty;
    len++;
    while(len < 5) {
        i = 0;
        while(elements[i].difficulty >= (DD - diff) && i < N)
            i++;
        if(i >= N)
            break;
        while(!ELEMcompatibility(elements[index[len]], elements[i]) && i < N)
            i++;
        if(i >= N)
            break;
        index[len] = i;
        len++;
        diff+= elements[i].difficulty;
    }

    prog.diagonals[1] = DIAGcompose(index, len);
    len = i = 0;

    // 3 è la diagonale migliore diagonale che iresce a mettere un DE >= 8 in ultima posizione
    maxdd = DD;
    diff = 0;
    if (elements[0].difficulty > 8) {
        diff = elements[0].difficulty;
        maxdd = DD - diff;
        while(len < 4) {
            i = 0;
            while(elements[i].difficulty >= maxdd && i<N)
                i++;
            if(i >= N)
                break;
            while(!ELEMcompatibility(elements[index[len]], elements[i]) && i < N)
                i++;
            if(i >= N)
                break;
            index[len] = i;
            len++;
            diff+= elements[i].difficulty;
        }
        i = 0;
        while(!ELEMcompatibility(elements[i], elements[index[len]]))
            i++;
        index[len] = i;
        len++;
    } else {
        maxdd = DD;
        diff = 0;
        while(elements[i].type != acr_front)
            i++;
        index[len] = i;
        diff+= elements[i].difficulty;
        len++;
        while(len < 5) {
            i = 0;
            while(elements[i].difficulty >= (maxdd - diff) && i<N)
                i++;
            if(i >= N)
                break;
            while(!ELEMcompatibility(elements[index[len]], elements[i]) && i < N)
                i++;
            if(i >= N)
                break;
            index[len] = i;
            len++;
            diff+= elements[i].difficulty;
        }
    }
    prog.diagonals[2] = DIAGcompose(index, len);

    printf("Migliore programma greedy: valore: %.2f\n\n", PROGevaluate(prog, elements));
    for (int i = 0; i < 3; i++){
        DIAGprint(prog.diagonals[i], elements);
    }

    return 0;
}

void ElemOrder(Element *elems, int N) {
    Element t;
    for (int i = 0; i < N - 1; i++) {
        for (int j = i+1; j < N; j++) {
            if (elems[j].value > elems[i].value) {
                t = elems[i];
                elems[i] = elems[j];
                elems[j] = t;
            }
        }
    }
}

int min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}
