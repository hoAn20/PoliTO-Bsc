#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_TIPI_PIETRE 4

enum {zaffiro, rubino, topazio, smeraldo};

void leggiFile(FILE *f, int **limiti);
int creaCollana(int *collana, int *limiti, int *valori, int *lunghezza, int *valore, int max_rip);
void stampaCollana(int *collana, int lunghezza);

int crea_collana( int *att, int *lim, int *qta, int prof, int pos, int max_rip);
int pietra_adatta(int pre, int att);
int continuabile(int att, int *qta, int *lim);
int valuta_collana(int *qta, int *valori);
int conta_rip(int *ptr, int att, int max_rip);

int main() {
    clock_t inizio = clock();
    FILE *fin = fopen("test_set.txt", "r");

    int *collana, *limiti, *valori;
    int lunghezza, valore, max_rip, righe;

    if (fin == NULL) {
        printf("Errore durante l' apertura del file.\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d", &righe);

    for (int i = 0; i < righe; i++) {
        leggiFile(fin, &limiti);
        leggiFile(fin, &valori);
        fscanf(fin, "%d", &max_rip);

        printf(" - COLLANA #%d -\n"
               "Pietre a disposizione: z = %d, r = %d, t = %d, s = %d\n"
               "Valori: z = %d, r = %d, t = %d, s = %d\n"
               "Massime ripetizioni: %d\n",
               (i+1), limiti[zaffiro], limiti[rubino], limiti[topazio], limiti[smeraldo],
               valori[zaffiro], valori[rubino], valori[topazio], valori[smeraldo],
               max_rip
        );

        collana = (int*)malloc(100*sizeof(int));
        creaCollana(collana, limiti, valori, &lunghezza, &valore, max_rip);

        printf("Lunghezza: %d\n"
               "Valore: %d\n"
               "Composizione: ",
               lunghezza, valore);
        stampaCollana(collana, lunghezza);
        printf("\n");
    }
    clock_t fine = clock();
    double tempo = (double)(fine - inizio) / 1e6;
    printf("Tempo di esecuzione: %.2fs", tempo);

    return 0;
}

void leggiFile(FILE *f, int **limiti) {
    int *lim = (int *)malloc(N_TIPI_PIETRE * sizeof(int));

    for (int i = 0; i < N_TIPI_PIETRE; i++)
        fscanf(f, "%d", &lim[i]);

    *limiti = lim;
}
int creaCollana(int *collana, int *limiti, int *valori, int *lunghezza, int *valore, int max_rip) {
    int *attuale, *qta;
    int tot_pietre = 0, max_val = 0;
    *valore = 0;

    qta = (int *)malloc(N_TIPI_PIETRE * sizeof(int));
    if (limiti[zaffiro] > limiti[smeraldo]) //nel caso in cui il numero di zaffiri sia a priori maggiore di quello di smeraldi li vado a livellare per le condizioni iniziali
        limiti[zaffiro] = limiti[smeraldo];
    for (int i = 0; i < N_TIPI_PIETRE; i++) {
        tot_pietre += limiti[i];
        if (valori[i] > max_val)
            max_val = valori[i];
    }
    attuale = (int *)malloc(tot_pietre * sizeof(int));
    *valore = 0;

    for (int k = tot_pietre; k > 1; k--) {
        for (int i = 0 ; i < N_TIPI_PIETRE; i++)
            qta[i] = 0;
        if (k * max_val < *valore)  //identificato il valore massimo tra le pietre moltiplicato per la lughezza non supera quello che è il massimo valore già ottenuto in ogi caso non è possibile creare collane con un valore maggiore di quello
            return 1;
        if (crea_collana(attuale, limiti, qta, k, 0, max_rip) && qta[zaffiro] <= qta[smeraldo] && valuta_collana(qta, valori) > *valore){
            for (int j =0; j < k; j++)
                collana[j] = attuale[j];
            *lunghezza = k;
            *valore = valuta_collana(qta, valori);
        }
    }
    return 0;
}
void stampaCollana(int *collana, int lunghezza) {
    for (int i = 0; i < lunghezza; i++) {
        switch(collana[i]) {
            case zaffiro:
                printf("z");
                break;
            case rubino:
                printf("r");
                break;
            case topazio:
                printf("t");
                break;
            case smeraldo:
                printf("s");
                break;
        }
    }
    printf("\n\n\n");
}

int crea_collana( int *att, int *lim, int *qta, int prof, int pos, int max_rip) {
    int cnt;
    if (pos == prof) {
        return 1;
    }
    for (int i = 0; i < N_TIPI_PIETRE; i++) {
        if (pos == 0) {
            att[pos] = i;
            qta[i]++;
            if (crea_collana(att, lim, qta, prof, pos + 1, max_rip))
                return 1;
            qta[i]--;
        } else {
            if (pos >= max_rip){
                cnt = conta_rip((att+pos-1), i, max_rip);
            } else
                cnt = 0;
            if (cnt <= max_rip && qta[i] < lim[i] && pietra_adatta(att[pos - 1], i) && continuabile(att[pos], qta, lim)) {
                att[pos] = i;
                qta[i]++;
                if (crea_collana(att, lim, qta, prof, pos + 1, max_rip))
                    return 1;
                qta[i]--;
            }
        }
    }
    return 0;
}
int pietra_adatta(int pre, int att) {
    switch(pre) {
        case zaffiro:
        case topazio:
            if (att != zaffiro && att != rubino)
                return 0;
            break;
        case rubino:
        case smeraldo:
            if (att != smeraldo && att!= topazio)
                return 0;
            break;
        default:
            return 0;
    }
    return 1;
}
int continuabile(int att, int *qta, int *lim) {
    switch(att) {
        case zaffiro:
        case topazio:
            if (qta[zaffiro] >= lim[zaffiro] && qta[rubino] >= lim[rubino])
                return 0;
            break;
        case rubino:
        case smeraldo:
            if (qta[smeraldo] >= lim[smeraldo] && qta[topazio] >= lim[topazio])
                return 0;
            break;
        default:
            return 0;
    }
    return 1;
}
int valuta_collana(int *qta, int *valori) {
    int valore = 0;
    for (int i =0; i < N_TIPI_PIETRE; i++)
        valore += qta[i] * valori[i];

    return valore;
}
int conta_rip(int *ptr, int att, int max_rip) {
    int i;
    for (i = 0; *(ptr-i) == att && i < max_rip; i++);
    return i+1;
}
