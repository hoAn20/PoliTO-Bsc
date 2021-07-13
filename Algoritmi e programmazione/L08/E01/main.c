#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int s, e, d;
    int used;
} Activity;

void attSel(int N, Activity *a);
void attSel_r(int k, int N, Activity *a, int *as, int *bs, int *max_d);
int ok(Activity *a, int *as, int N);
int valuta(Activity *a, int *as, int N);

int main() {
    FILE *factivity = fopen("att.txt", "r");
    int N, i;
    Activity *activities;

    if (factivity == NULL) {
        printf("Errore durante l'apertura del file.\n");
        return EXIT_FAILURE;
    }

    fscanf(factivity, "%d", &N);
    activities = malloc(N * sizeof(Activity));
    for (i = 0; i < N; i++) {
        fscanf(factivity, "%d %d", &activities[i].s, &activities[i].e);
        activities[i].d = activities[i].e - activities[i].s;
    }

    attSel(N, activities);

    free(activities);
    return 0;
}

void attSel(int N, Activity *a) {
    int i, *attsel, *bestsel, max_time = 0;
    attsel = malloc(N * sizeof(int));   //attsel contine 0 se l'attività non è presa e 1 se l'attività è invece presa
    bestsel = malloc(N * sizeof(int));

    attSel_r(0, N, a, attsel, bestsel, &max_time);

    printf("La soluzione massima ha durata: %d\n\nContiene le attivita':\n", max_time);
    for (i = 0; i < N; i ++) {
        if (bestsel[i])
            printf(" %d", i);
    }
    printf("\n");

    free(bestsel);
    free(attsel);
}
void attSel_r(int k, int N, Activity *a, int *as, int *bs, int *max_d) {
    int d, i;

    if (k >= N) {
        if (ok(a, as, N)) {
            d = valuta(a, as, N);
            if (d > *max_d) {
                *max_d = d;
                for (i = 0; i < N; i++) {
                    bs[i] = as[i];
                }
            }
        }
    } else {
        as[k] = 1;
        attSel_r(k + 1, N, a, as, bs, max_d);

        as[k] = 0;
        attSel_r(k + 1, N, a, as, bs, max_d);
    }
}
//questa funzione controlla che per ogni attività selezionata la fine della precedente sia <= dell'inizio della seguente
int ok(Activity *a, int *as, int N) {
    int i, last_e = 0;
    for (i = 0; i < N; i++) {
        if (as[i]) {
            if (last_e > a[i].s)
                return 0;
            else
                last_e = a[i].e;
        }
    }
    return 1;
}
//questa funzione conta quanti slot di tempo vengono coperti dalle attività selezionate
int valuta(Activity *a, int *as, int N){
    int d = 0, i;
    for (i = 0; i < N; i++) {
        if (as[i]) {
            d+= a[i].d;
        }
    }
    return d;
}
