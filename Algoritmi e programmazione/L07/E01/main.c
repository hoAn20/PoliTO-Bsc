#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_TIPI_PIETRE 4

enum {zaffiro, rubino, topazio, smeraldo};

void leggiFile(FILE *f, int **limiti);
int creaCollana(int **collana, int *limiti, int *lunghezza);
void stampaCollana(int *collana, int lunghezza);

int crea_collana( int *att, int *lim, int *qta, int prof, int pos);
int pietra_adatta(int pre, int att);
int continuabile(int att, int *qta, int *lim);

int main() {
    clock_t inizio = clock();
    FILE *fin = fopen("test_easy.txt", "r");
    //FILE *fin = fopen("test_hard.txt", "r");

    int *collana, *limiti;
    int lunghezza, righe;

    if (fin == NULL) {
        printf("Errore durante l' apertura del file.\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d", &righe);

    for (int i = 0; i < righe; i++) {
        leggiFile(fin, &limiti);

        printf(" - COLLANA #%d -\n"
               "Pietre a disposizione: z = %d, r = %d, t = %d, s = %d\n",
               (i+1), limiti[0], limiti[1], limiti[2], limiti[3]
        );

        creaCollana(&collana, limiti, &lunghezza);

        printf("Lunghezza: %d\n"
               "Composizione: ",
               lunghezza);

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
int creaCollana(int **collana, int *limiti, int *lunghezza) {
    int *attuale, *qta;
    int tot_pietre = 0;

    qta = (int *)malloc(N_TIPI_PIETRE * sizeof(int));
    for (int i = 0; i < N_TIPI_PIETRE; i++)
        tot_pietre += limiti[i];
    attuale = (int *)malloc(tot_pietre * sizeof(int));

    for (int k = tot_pietre; k > 1; k--) {
        for (int i = 0 ; i < N_TIPI_PIETRE; i++)
            qta[i] = 0;
        if (crea_collana(attuale, limiti, qta, k, 0)){
            *collana = (int *)malloc(k * sizeof(int));
            *collana = attuale;
            *lunghezza = k;
            return 1;
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

int crea_collana( int *att, int *lim, int *qta, int prof, int pos) {
    if (pos == prof) {
        return 1;
    }
    for (int i = 0; i < N_TIPI_PIETRE; i++) {
        if (pos == 0) {
            att[pos] = i;
            qta[i]++;
            if (crea_collana(att, lim, qta, prof, pos + 1))
                return 1;
            qta[i]--;
        } else {
            if (qta[i] < lim[i] && pietra_adatta(att[pos-1], i) && continuabile(i, qta, lim)) {
                att[pos] = i;
                qta[i]++;
                if (crea_collana(att, lim, qta, prof, pos + 1))
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
            if (qta[zaffiro]+1 >= lim[zaffiro] && qta[rubino] >= lim[rubino])
                return 0;
            break;
        case rubino:
        case smeraldo:
            if (qta[smeraldo]+1 >= lim[smeraldo] && qta[topazio] >= lim[topazio])
                return 0;
            break;
        default:
            return 0;
    }
    return 1;
}
