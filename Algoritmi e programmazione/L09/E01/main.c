#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int s, e;
    int d;
} Activity;

int compatibile (Activity a, Activity b);
void solveDyn(Activity *a, int N);
void mergeSort(Activity *a, int N);
void mergeSortR(Activity *a, Activity *b, int l, int r);
void merge(Activity *a, Activity *b, int l, int q, int r);

int main() {
    FILE *factivity = fopen("att2.txt", "r");
    int N;
    Activity *activities;

    if (factivity == NULL) {
        printf("Errore nell'apertura del file.\n");
        return EXIT_FAILURE;
    }

    fscanf(factivity, "%d", &N);
    N++;
    activities = malloc(N * sizeof(Activity));
    activities[0].s = activities[0].e = activities[0].d = 0;
    for (int i = 1; i < N ; i++) {
        fscanf(factivity, "%d %d", &activities[i].s, &activities[i].e);
        activities[i].d = activities[i].e - activities[i].s;
    }

    mergeSort(activities, N);

    solveDyn(activities, N);

    free(activities);
    return 0;
}

//algoritmo di merge sort per riordinare le attività in base ai tempi di inizio, a parità d questi in ordine di durata crescente
void mergeSort(Activity *a, int N) {
    int l = 1, r = N-1;
    Activity *b = malloc(N * sizeof(Activity));
    mergeSortR(a, b, l, r);
}
void mergeSortR(Activity *a, Activity *b, int l, int r) {
    int q = (l+r)/2;
    if (r <= l)
        return;
    mergeSortR(a, b, l, q);
    mergeSortR(a, b, q+1, r);
    merge(a, b, l, q, r);
}
void merge(Activity *a, Activity *b, int l, int q, int r) {
    int i, j, k;
    i = l;
    j = q+1;
    for (k = l; k <= r; k++) {
        if (i > q)
            b[k] = a[j++];
        else if(j > r)
            b[k] = a[i++];
        else if (a[j].s > a[i].s)
            b[k] = a[i++];
        else if (a[j].s == a[i].s && a[j].e > a[i].e)
            b[k] = a[i++];
        else
            b[k] = a[j++];
    }
    for (k = l; k <= r; k++) {
        a[k] = b[k];
    }
}
int compatibile (Activity a, Activity b) {
    if (b.s >= a.e)
        return 1;
    else
        return 0;
}
void solveDyn(Activity *a, int N) {
    int *opt, *pre, ind, pre_max = INT_MIN, ind_pre_max = 1, ind_MAX;

    opt = calloc(N, sizeof(int));
    pre = calloc(N, sizeof(int));

    opt[1] = a[1].d;
    pre[1] = 0;
    ind_MAX = 1;
    for (int i = 2; i < N; i++) {
        cnt++;
        ind = i-1;
        while(!compatibile(a[ind], a[i])) {
            ind--;
            cnt++;
        }
        for (int j = ind_pre_max; j <= ind; j++) {
            cnt++;
            if (opt[j] >= pre_max) {
                ind_pre_max = j;
                pre_max = opt[j];
            }
        }
        pre[i] = ind_pre_max;
        opt[i] = opt[pre[i]] + a[i].d;
        if (opt[i] >= opt[ind_MAX])
            ind_MAX = i;
    }

    printf("La soluzione massima ha durata: %d\n\nContiene le attivita':\n", opt[ind_MAX]);
    while(ind_MAX != 0) {
        printf("%d ", ind_MAX);
        ind_MAX = pre[ind_MAX];
    }
    printf("\n");
}
