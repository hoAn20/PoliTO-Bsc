
//L01E01

/*
 * FORMATO FILE
 *
 * nome file inserito dall'utente, massimo 20 caratteri
 *
 * n_squadre n_giornate
 * risultati
 *
 * esempio: risultati.txt
 *
 * 4 8
 * 1 3 0 1 3 0 1 1
 * 1 1 1 1 1 1 1 1
 * 1 0 3 1 0 3 1 1
 * 1 1 1 1 1 1 1 1
 */

#include <stdio.h>
#include <stdlib.h>

const int MAX_N_TEAM = 20;
const int MAX_M_DAYS = 20;
const int MAX_STR_LEN = 20;

int findLeader(int res[MAX_N_TEAM][MAX_M_DAYS], int nT, int nD, int p[MAX_N_TEAM]);

int main() {
    FILE *fileChampionship;
    int results[MAX_N_TEAM][MAX_M_DAYS], nTeams, nDays, points[MAX_N_TEAM];
    char fileName[MAX_STR_LEN+1];
    int i, j;

	//inizializzazione del vettore contenente i punti di ogni squadra a 0 in modo da poter fare direttamente la somma
    for(i = 0; i < MAX_N_TEAM; i++) {
        points[i] = 0;
    }

    //lettura del nome del file e successiva
    printf("Inserisci il nome del file da cui caricare i risultati: ");
    gets(fileName);
    if((fileChampionship = fopen(fileName, "r")) == NULL) {
        //gestione di un errore nell'apertura del file
        printf("Si %c verificato un errore nell'apertura del file. Riprovare.\n\n", 138);
        system("pause");
        return EXIT_FAILURE;
    }

    //lettura da file degli effettii valori di n e m
    fscanf(fileChampionship, "%d %d", &nTeams, &nDays);
    if(nTeams > MAX_N_TEAM || nTeams < 0 || nDays > MAX_M_DAYS || nDays < 0) {
        //gestione di un errore nelle dimensioni effettive specificate
        printf("Almeno una delle dimensioni lette da file non rispetta le dimensioni massime assegnate o è negativa.\n");
        return EXIT_FAILURE;
    }
    //lettura dei risultati riga per riga (cioè squadra per squadra)
    for(i = 0; i < nTeams; i++) {
        for(j = 0; j < nDays; j++) {
            fscanf(fileChampionship, "%d", &results[i][j]);
        }
    }
    fclose(fileChampionship);

    //scrittura del capolista in ogni giornata sfruttando la funzione findLeader
    for(i = 0; i < nDays; i++) {
        printf("La capolista della giornata %d %c: %d\n\n", (i + 1), 138, findLeader(results, nTeams, i, points));
    }

    system("pause");
    return 0;
}

/*
 * La funzione va ad aggiornare il punteggio della squadra in base al risultato della giornata in corso e nel caso il
 * puntaggio superi quello del capolista indica come indice del capolista quello della squadra che si stava leggendo.
 * Nel caso ci siano più squadre con lo stesso punteggio pari al massimo, verrà indicata come capolista quella con
 * l'indice maggiore.
 *
 * res: matrice con i risultati di tutte le squadre
 * nT:  numero totale delle squadre del campionato
 * nD:  numero delle giornate di cui sono salvati i risultati nella matrice
 * p:   vettore contenente i punti di ciascuna squadra
 *
 * return: indice della squadra capolista
 */
int findLeader(int res[MAX_N_TEAM][MAX_M_DAYS], int nT, int nD,int p[MAX_N_TEAM]) {
    int i, iLeader = 0;

    for(i = 0; i < nT; i++) {
        if(res[i][nD] == 3) {
            p[i] += 3;
        } else if(res[i][nD] == 1) {
            p[i] += 1;
        }
        if(p[i] > p[iLeader]) {
            iLeader = i;
        }
    }

    return iLeader;
}
