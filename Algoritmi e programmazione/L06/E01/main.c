#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v1;
    int v2;
} arco;

arco* leggiFile(int *n, int *e);
void insiemeParti(arco* archi, int pos, int n, int e, int *valore, int *preso);
int soluzioneValida(arco *archi, int *preso, int *valore, int n, int e);
void stampaSoluzione(int *preso, int *valore, int n);

int main() {
    arco *archi;
    int N, E;
    int *valore, *preso;

    archi = leggiFile(&N, &E);

    valore = malloc(N * sizeof(int));
    preso = calloc(N, sizeof(int));

    for (int i = 0; i < N; i++)
        valore[i] = i;

    insiemeParti(archi, 0, N, E, valore, preso);

    return 0;
}

/*
legge da un file specificato il numero di nodi e lati di un grafo e i lati effettivi. Salva questi dati in un
vettore allocato dinamicamente
*/
arco* leggiFile(int *n, int *e) {
    arco* archi;
    FILE *fin;
    char nomefile[21];

    printf("Inserisci il nome del file da leggere: ");
    gets(nomefile);
    fflush(stdin);

    if ((fin = fopen(nomefile, "r")) == NULL) {
        printf("Impossibile aprire il file specificato.\n");
        exit(-1);
    }
    fscanf(fin, "%d %d", n, e);
    archi = malloc(*e * sizeof(arco));
    for (int i = 0; i < *e; i++)
        fscanf(fin, "%d %d", &archi[i].v1, &archi[i].v2);

    fclose(fin);
    return archi;
}

/*
il modello di generazione del vertex cover deve coprire tutti i possibili sottoinsiemi di nodi quindi si sfrutta
il modello dell'insieme delle parti.

archi : insieme degli archi letti dal file 
pos : nodo considerato
n : numero di nodi
e : numero di archi
valore : nome dei vari nodi
preso : vettore con cui si lavora sulla generazione dell'insieme delle parti
*/
void insiemeParti(arco* archi, int pos, int n, int e, int *valore, int *preso) {
    if (pos >= n) {
        if (soluzioneValida(archi, preso, valore, n, e))
            stampaSoluzione(preso, valore, n);
        return;
    }
    preso[pos] = 1;
    insiemeParti(archi, pos+1, n, e, valore, preso);
    preso[pos] = 0;
    insiemeParti(archi, pos+1, n, e, valore, preso);
}

/*
funzione che controlla la validità della soluzione ottenuta con l'insieme delle parti andando a controllare il vertexcover
*/
int soluzioneValida(arco *archi, int *preso, int *valore, int n, int e) {
    int valida;
    for (int i = 0; i < e; i++) {
        valida = 0;
        for (int j = 0; j < n; j++) {
            if (preso[j] && (valore[j] == archi[i].v1 || valore[j] == archi[i].v2))
                valida = 1;
        }
        if (!valida)
            return valida;
    }
    return valida;
}
//funzione che stampa la soluzione se soluzioneValida è vero
void stampaSoluzione(int *preso, int *valore, int n) {
    printf("{");
    for (int i = 0; i < n; i++) {
        if (preso[i])
            printf("%d, ", valore[i]);
    }
    printf("}\n");
}