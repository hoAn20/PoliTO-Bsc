#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 20
#define MAX_DATA 100
#define N_ORDINAMENTI 4

typedef enum {sv, sf, rp, od, oc, op, oa, fine} comando;

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

comando leggiComando();
void elaborazioneDati(corsa *lista, int nDati, comando comandoLetto);
void scambia(corsa *c1, corsa *c2);
void stampa(FILE *f, corsa *lista, int nDati);
void stampaDaPuntatori(corsa **lista, int nDati);
void copiaVettorePerPuntatori(corsa *lista1, corsa **lista2, int nDati);
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
    corsa lista[MAX_DATA];
    int nDati;

    if ((fin = fopen("corse.txt", "r")) == NULL) {
        printf("Impossibile leggere il file corse.txt\n\n");
        return EXIT_FAILURE;
    }
    fscanf(fin, "%d", &nDati);
    for (int i = 0; i < nDati; i++)
        fscanf(fin, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", lista[i].codice, lista[i].p_partenza, lista[i].p_arrivo, &lista[i].data_corsa.g, &lista[i].data_corsa.m, &lista[i].data_corsa.a, &lista[i].o_partenza.o, &lista[i].o_partenza.m, &lista[i].o_partenza.s, &lista[i].o_arrivo.o, &lista[i].o_arrivo.m, &lista[i].o_arrivo.s, &lista[i].ritardo);
    fclose(fin);

    stampa(stdout, lista, nDati);

    do {
        comandoLetto = leggiComando();
        elaborazioneDati(lista, nDati, comandoLetto);
    } while(comandoLetto != fine);

    return 0;
}

/*
La funzione leggiComando acquisisce una stringa e controlla se sia la stringa che rappresenta un coando e in tal caso restituisci il valore
corrispondente ad un enum.
*/
comando leggiComando() {
    char stringaDiComando[MAX_STR_LEN+1];
    printf("\n\nOpzioni disponibili:\n"
           "stampa -v/-f : su video/ file\n"
           "ricerca -p : in base alla stazione di partenza\n"
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

/*
La funzione scambia scambia i valori delle due struct, per riordinarle.
*/
void scambia(corsa *c1, corsa *c2) {
    corsa temp = *c1;
    *c1 = *c2;
    *c2 = temp;
}

/*
La funzione elaborazioneDati serve a far eseguire alprogramma la giusta funzione per il comando letto con leggiComando
allocando dinamicamente i vettori di puntatori contenenti i vettori ordinati

lista : lista originale letta dal file
nDati : numero di dati letti
comandoLetto : comando letto 
*/
void elaborazioneDati(corsa *lista, int nDati, comando comandoLetto) {
    FILE *fout;
    char nomeFile[MAX_STR_LEN+1], partenzaCercata[MAX_STR_LEN+1];
    static corsa *listaOrdinata[N_ORDINAMENTI][MAX_DATA]; //0 data / 1 codice / 2 partenza / 3 arrivo
    static int ordinamentoEffettutato[N_ORDINAMENTI] = {0};
    int pos;

    switch (comandoLetto) {
        case sv :
            stampa(stdout, lista, nDati);
            break;
        case sf:
            printf("Specificare il file su cui stampare: ");
            gets(nomeFile);
            fflush(stdin);
            fout = fopen(nomeFile, "w");
            stampa(fout, lista, nDati);
            fclose(fout);
            break;
        case od:
            if (!ordinamentoEffettutato[0]) {
                ordinamentoEffettutato[0] = 1;
                copiaVettorePerPuntatori(lista, listaOrdinata[0], nDati);
                bubbleSort(listaOrdinata[0], nDati, &dataGT);
            }
            stampaDaPuntatori(listaOrdinata[0], nDati);
            break;
        case oc:
            if (!ordinamentoEffettutato[1]) {
                ordinamentoEffettutato[1] = 1;
                copiaVettorePerPuntatori(lista, listaOrdinata[1], nDati);
                bubbleSort(listaOrdinata[1], nDati, &codiceGT);
            }
            stampaDaPuntatori(listaOrdinata[1], nDati);
            break;
        case op:
            if (!ordinamentoEffettutato[2]) {
                ordinamentoEffettutato[2] = 1;
                copiaVettorePerPuntatori(lista, listaOrdinata[2], nDati);
                bubbleSort(listaOrdinata[2], nDati, &partenzaGT);
            }
            stampaDaPuntatori(listaOrdinata[2], nDati);
            break;
        case oa:
            if (!ordinamentoEffettutato[3]) {
                ordinamentoEffettutato[3] = 1;
                copiaVettorePerPuntatori(lista, listaOrdinata[3], nDati);
                bubbleSort(listaOrdinata[3], nDati, &arrivoGT);
            }
            stampaDaPuntatori(listaOrdinata[3], nDati);
            break;
        case rp:
            printf("Inserire la stazione di partenza ricercata: ");
            gets(partenzaCercata);
            fflush(stdin);
            if (ordinamentoEffettutato[2])
                pos = ricercaDicotomicaPartnza(listaOrdinata[2], 0, nDati, partenzaCercata);
            else
                pos = ricercaLinearePartenza(lista, nDati, partenzaCercata);
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

/*
Funzione di stampa che può essere chiamata da elaborazioneDati sia con stdin che con un file aperto per scriverci sopra
*/
void stampa(FILE *f, corsa *lista, int nDati) {
    for (int i = 0; i < nDati; i++)
        fprintf(f, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", lista[i].codice, lista[i].p_partenza, lista[i].p_arrivo, lista[i].data_corsa.g, lista[i].data_corsa.m, lista[i].data_corsa.a, lista[i].o_partenza.o, lista[i].o_partenza.m, lista[i].o_partenza.s, lista[i].o_arrivo.o, lista[i].o_arrivo.m, lista[i].o_arrivo.s, lista[i].ritardo);
}

/*
Stampa del vettore ordinato secondo i vettori di puntatori ottenuti per ordinamento
*/
void stampaDaPuntatori(corsa **lista, int nDati) {
    printf("Lista riordinata: \n");
    for (int i = 0; i < nDati; i++)
        printf("%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d\n", lista[i]->codice, lista[i]->p_partenza, lista[i]->p_arrivo, lista[i]->data_corsa.g, lista[i]->data_corsa.m, lista[i]->data_corsa.a, lista[i]->o_partenza.o, lista[i]->o_partenza.m, lista[i]->o_partenza.s, lista[i]->o_arrivo.o, lista[i]->o_arrivo.m, lista[i]->o_arrivo.s, lista[i]->ritardo);
}

/*
Copia il vettore dalla lista originale nel vettore di puntatori per ordinamento in modo da lavorare solo su quelli già
durante gli ordinamenti.
*/
void copiaVettorePerPuntatori(corsa *lista1, corsa **lista2, int nDati) {
    for (int i = 0; i < nDati; i++)
        lista2[i] = &lista1[i];
}

/*
Funzione che implementa la funzione di ordinamento bubblesort direttamente sui vettori di puntatori e sfrutta una funzione
di maggiore o uguale passata per parametro per non dover fare un bubblsort per ogni ordinamenti richiesto.
*/
void bubbleSort(corsa **lista, int nDati, int (*fnGT)(corsa c1, corsa c2)) {
    int i, j, l = 0, r = nDati-1, scambioEffettuato = 1;
    corsa *temp;

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

//funzione di maggiore o uguale per date (e eventualmente ore)
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

//funzione di maggiore o uguale per il codice della corsa
int codiceGT(corsa c1, corsa c2) {
    if (strcmp(c1.codice, c2.codice) > 0)
        return 1;
    return 0;
}

//funzione di magiore o uguale per la stazione di partenza
int partenzaGT(corsa c1, corsa c2) {
    if (strcmp(c1.p_partenza, c2.p_partenza) > 0)
        return 1;
    return 0;
}

//funzione di maggiore o ugual per la stazione di arrivo
int arrivoGT(corsa c1, corsa c2) {
    if (strcmp(c1.p_arrivo, c2.p_arrivo) > 0)
        return 1;
    return 0;
}

//funzione di ricerca lineare sulle partenze
int ricercaLinearePartenza(corsa *lista, int nDati, char *partenzaCercata) {
    for (int i = 0; i < nDati; i++) {
        if (strcmp(lista[i].p_partenza, partenzaCercata) == 0)
            return i;
    }
    return -1;
}

//funzione di ricerca dicotomica sulle stazioni di partenza
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