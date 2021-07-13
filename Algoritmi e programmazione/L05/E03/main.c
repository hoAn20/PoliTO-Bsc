#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 20
#define MAX_DATA 100
#define N_ORDINAMENTI 4

typedef enum {lf, sv, sf, rp, od, oc, op, oa, fine} comando;

typedef struct {
    int g;
    int m;
    int a;
} data;
typedef struct {
    int s;
    int m;
    int o;
} orario;
typedef struct {
    char codice[MAX_STR_LEN+1];
    char p_partenza[MAX_STR_LEN+1];
    char p_arrivo[MAX_STR_LEN+1];
    data data_corsa;
    orario o_partenza;
    orario o_arrivo;
    int ritardo;
} corsa;

corsa* leggiFile(FILE *f, int *nDati);
comando leggiComando();
void elaborazioneDati(corsa **lista, int *nDati, comando comandoLetto);

void stampa(FILE *f, corsa *lista, int nDati);
void stampaDaPuntatori(corsa **lista, int nDati);
void copiaVettorePerPuntatori(corsa *lista1, corsa **lista2, int nDati);
void scambia(corsa *c1, corsa *c2);
void bubbleSort(corsa **lista, int nDati, int (*fnGT)(corsa c1, corsa c2));
int dataGT(corsa c1, corsa c2);
int codiceGT(corsa c1, corsa c2);
int partenzaGT(corsa c1, corsa c2);
int arrivoGT(corsa c1, corsa c2);
int ricercaLinearePartenza(corsa *lista, int nDati, char *partenzaCercata);
int ricercaDicotomicaPartnza(corsa **lista, int l, int r, char *partenzaCercata);

int main() {
    FILE *fin;
    comando comandoLetto;
    corsa *lista;
    int nDati;

    if ((fin = fopen("corse.txt", "r")) == NULL) {
        printf("Impossibile leggere il file corse.txt\n\n");
        return EXIT_FAILURE;
    }
    lista = leggiFile(fin, &nDati);
    fclose(fin);

    stampa(stdout, lista, nDati);

    do {
        comandoLetto = leggiComando();
        elaborazioneDati(&lista, &nDati, comandoLetto);
    } while(comandoLetto != fine);

    return 0;
}

/*
funzione che va a leggere dal file le corse e le ritorna in un vettore insieme al numero di dati letti
in questo modo è possibile leggere più file durante un'unica esecuzione del programma
*/
corsa* leggiFile(FILE *f, int *nDati) {
    corsa *lista;
    fscanf(f, "%d", nDati);
    lista = malloc(*nDati * sizeof(corsa));
    for (int i = 0; i < *nDati; i++)
        fscanf(f, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", lista[i].codice, lista[i].p_partenza, lista[i].p_arrivo, &lista[i].data_corsa.g, &lista[i].data_corsa.m, &lista[i].data_corsa.a, &lista[i].o_partenza.o, &lista[i].o_partenza.m, &lista[i].o_partenza.s, &lista[i].o_arrivo.o, &lista[i].o_arrivo.m, &lista[i].o_arrivo.s, &lista[i].ritardo);
    return lista;
}
comando leggiComando() {
    char stringaDiComando[MAX_STR_LEN+1];
    printf("\n\nOpzioni disponibili:\n"
           "stampa -v/-f : su video/ file\n"
           "ricerca -p : in base alla stazione di partenza\n"
           "leggi -f : leggi da file una nuova lista\n"
           "ordinamento -d/-c/-p/-a : in base a data/codice tratta/partenza/arrivo\n"
           "fine : per terminare\n\n"
           "Inserisci il comando: ");
    gets(stringaDiComando);
    if (strcmp(stringaDiComando, "stampa -v") == 0)
        return sv;
    else if (strcmp(stringaDiComando, "stampa -f") == 0)
        return sf;
    else if (strcmp(stringaDiComando, "ricerca -p") == 0)
        return rp;
    else if (strcmp(stringaDiComando, "leggi -f") == 0)
        return lf;
    else if (strcmp(stringaDiComando, "ordinamento -d") == 0)
        return od;
    else if (strcmp(stringaDiComando, "ordinamento -c") == 0)
        return oc;
    else if (strcmp(stringaDiComando, "ordinamento -p") == 0)
        return op;
    else if (strcmp(stringaDiComando, "ordinamento -a") == 0)
        return oa;
    else if (strcmp(stringaDiComando, "fine") == 0)
        return fine;
    else
        return -1;
}
void elaborazioneDati(corsa **lista, int *nDati, comando comandoLetto) {
    FILE *fin, *fout;
    char nomeFile[MAX_STR_LEN+1], partenzaCercata[MAX_STR_LEN+1];
    static corsa **listaOrdinata[N_ORDINAMENTI]; //0 data / 1 codice / 2 partenza / 3 arrivo
    static int ordinamentoEffettutato[N_ORDINAMENTI];
    int pos;

    for (int i = 0; i < N_ORDINAMENTI; i++) {
        ordinamentoEffettutato[i] = 0;
        listaOrdinata[i] = malloc(*nDati * sizeof(corsa*));
    }
    switch (comandoLetto) {
        case lf:
            printf("Inserisci il nome del file da cui leggere: ");
            gets(nomeFile);
            fflush(stdin);
            if ((fin = fopen(nomeFile, "r")) == NULL) {
                printf("Impossibile leggere il file %s\n\n", nomeFile);
            }
            *lista = leggiFile(fin, nDati);
            for (int i = 0; i < N_ORDINAMENTI; i++) {
                ordinamentoEffettutato[i] = 0;
                listaOrdinata[i] = malloc(*nDati * sizeof(corsa*));
            }
            break;
        case sv :
            stampa(stdout, *lista, *nDati);
            break;
        case sf:
            printf("Specificare il file su cui stampare: ");
            gets(nomeFile);
            fflush(stdin);
            fout = fopen(nomeFile, "w");
            stampa(fout, *lista, *nDati);
            fclose(fout);
            break;
        case od:
            if (!ordinamentoEffettutato[0]) {
                ordinamentoEffettutato[0] = 1;
                copiaVettorePerPuntatori(*lista, listaOrdinata[0], *nDati);
                bubbleSort(listaOrdinata[0], *nDati, &dataGT);
            }
            stampaDaPuntatori(listaOrdinata[0], *nDati);
            break;
        case oc:
            if (!ordinamentoEffettutato[1]) {
                ordinamentoEffettutato[1] = 1;
                copiaVettorePerPuntatori(*lista, listaOrdinata[1], *nDati);
                bubbleSort(listaOrdinata[1], *nDati, &codiceGT);
            }
            stampaDaPuntatori(listaOrdinata[1], *nDati);
            break;
        case op:
            if (!ordinamentoEffettutato[2]) {
                ordinamentoEffettutato[2] = 1;
                copiaVettorePerPuntatori(*lista, listaOrdinata[2], *nDati);
                bubbleSort(listaOrdinata[2], *nDati, &partenzaGT);
            }
            stampaDaPuntatori(listaOrdinata[2], *nDati);
            break;
        case oa:
            if (!ordinamentoEffettutato[3]) {
                ordinamentoEffettutato[3] = 1;
                copiaVettorePerPuntatori(*lista, listaOrdinata[3], *nDati);
                bubbleSort(listaOrdinata[3], *nDati, &arrivoGT);
            }
            stampaDaPuntatori(listaOrdinata[3], *nDati);
            break;
        case rp:
            printf("Inserire la stazione di partenza ricercata: ");
            gets(partenzaCercata);
            fflush(stdin);
            if (ordinamentoEffettutato[2])
                pos = ricercaDicotomicaPartnza(listaOrdinata[2], 0, *nDati, partenzaCercata);
            else
                pos = ricercaLinearePartenza(*lista, *nDati, partenzaCercata);
            if (pos >= 0)
                printf("Prima occorrenza identificata in posizione %d.\n\n", pos);
            else
                printf("Elemento non trovato.\n\n");
            break;
        case fine:
            printf("Grazie per aver usato questo programma.\n\n");
            break;
        default:
            printf("INSERIRE UN COMANDO VALIDO.\n\n");
    }
}
void stampa(FILE *f, corsa *lista, int nDati) {
    for (int i = 0; i < nDati; i++)
        fprintf(f, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", lista[i].codice, lista[i].p_partenza, lista[i].p_arrivo, lista[i].data_corsa.g, lista[i].data_corsa.m, lista[i].data_corsa.a, lista[i].o_partenza.o, lista[i].o_partenza.m, lista[i].o_partenza.s, lista[i].o_arrivo.o, lista[i].o_arrivo.m, lista[i].o_arrivo.s, lista[i].ritardo);
}
void stampaDaPuntatori(corsa **lista, int nDati) {
    printf("Lista riordinata: \n");
    for (int i = 0; i < nDati; i++)
        printf("%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", lista[i]->codice, lista[i]->p_partenza, lista[i]->p_arrivo, lista[i]->data_corsa.g, lista[i]->data_corsa.m, lista[i]->data_corsa.a, lista[i]->o_partenza.o, lista[i]->o_partenza.m, lista[i]->o_partenza.s, lista[i]->o_arrivo.o, lista[i]->o_arrivo.m, lista[i]->o_arrivo.s, lista[i]->ritardo);
}
void copiaVettorePerPuntatori(corsa *lista1, corsa **lista2, int nDati) {
    for (int i = 0; i < nDati; i++)
        lista2[i] = &lista1[i];
}
void scambia(corsa *c1, corsa *c2) {
    corsa temp = *c1;
    *c1 = *c2;
    *c2 = temp;
}
void bubbleSort(corsa **lista, int nDati, int (*fnGT)(corsa c1, corsa c2)) {
    int i, j, l = 0, r = nDati-1, scambioEffettuato = 1;

    for (i = l; i < r && scambioEffettuato; i++) {
        scambioEffettuato = 0;
        for (j = l; j < r-i+l; j++) {
            if (fnGT(*lista[j], *lista[j+1])) {
                scambioEffettuato = 1;
                scambia(lista[j], lista[j+1]);
            }
        }
    }
}
int dataGT(corsa c1, corsa c2) {
    int d1, d2, o1, o2;
    d1 = c1.data_corsa.a*10000 + c1.data_corsa.m*100 + c1.data_corsa.g;
    d2 = c2.data_corsa.a*10000 + c2.data_corsa.m*100 + c2.data_corsa.g;
    if (d1 > d2)
        return 1;
    else if (d1 == d2) {
        o1 = c1.o_partenza.o*3600 + c1.o_partenza.m*60 + c1.o_partenza.s;
        o2 = c2.o_partenza.o*3600 + c2.o_partenza.m*60 + c2.o_partenza.s;
        if (o1 > o2)
            return 1;
    }
    return 0;
}
int codiceGT(corsa c1, corsa c2) {
    if (strcmp(c1.codice, c2.codice) > 0)
        return 1;
    return 0;
}
int partenzaGT(corsa c1, corsa c2) {
    if (strcmp(c1.p_partenza, c2.p_partenza) > 0)
        return 1;
    return 0;
}
int arrivoGT(corsa c1, corsa c2) {
    if (strcmp(c1.p_arrivo, c2.p_arrivo) > 0)
        return 1;
    return 0;
}
int ricercaLinearePartenza(corsa *lista, int nDati, char *partenzaCercata) {
    for (int i = 0; i < nDati; i++) {
        if (strcmp(lista[i].p_partenza, partenzaCercata) == 0)
            return i;
    }
    return -1;
}
int ricercaDicotomicaPartnza(corsa **lista, int l, int r, char *partenzaCercata) {
    int m = (l+r) / 2, differenzaStringhe = strcmp(lista[m]->p_partenza, partenzaCercata) == 0;
    if (l >= r && differenzaStringhe != 0)
        return -1;
    if (differenzaStringhe == 0)
        return m;
    else if (differenzaStringhe > 0)
        return ricercaDicotomicaPartnza(lista, m+1, r, partenzaCercata);
    else
        return ricercaDicotomicaPartnza(lista, l, m-1, partenzaCercata);
}