
//L02E02

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 1000
#define MAX_STR_LEN 30
#define MAX_COMMAND_LEN 12

typedef enum {r_date, r_partenza, r_capolinea, r_ritardo, r_ritardoTot, r_fine} command;

typedef struct {
    char code[MAX_STR_LEN];
    char startPoint[MAX_STR_LEN + 1];
    char endPoint[MAX_STR_LEN + 1];
    char date[MAX_STR_LEN + 1];
    char startTime[MAX_STR_LEN + 1];
    char endTime[MAX_STR_LEN + 1];
    int delay;
} line;

command readCommand();
void dates(line d[MAX_DATA], int nd);
void departures(line d[MAX_DATA], int nd);
void arrivals(line d[MAX_DATA], int nd);
void delay(line d[MAX_DATA], int nd);
void totalDelay(line d[MAX_DATA], int nd);

int main() {
    FILE *f;
    line data[MAX_DATA];
    int nData, i, continueLoop = 1;
    command selectedCommand;

    if((f = fopen("corse.txt", "r")) == NULL) {
        //gestione errore durante l'apertura del file
        printf("Non %c stato possibile trovare il file corse.txt . Riprovare.\n", 138);
        system("pause");
        return EXIT_FAILURE;
    }
    fscanf(f, "%d", &nData);

    for(i = 0; i < nData; i++) {
        fscanf(f, "%s %s %s %s %s %s %d", data[i].code, data[i].startPoint, data[i].endPoint, data[i].date, data[i].startTime, data[i].endTime, &data[i].delay);
    }

    do {
        selectedCommand = readCommand();
        switch(selectedCommand) {
            case r_date:
                dates(data, nData);
                break;
            case r_partenza:
                departures(data, nData);
                break;
            case r_capolinea:
                arrivals(data, nData);
                break;
            case r_ritardo:
                delay(data, nData);
                break;
            case r_ritardoTot:
                totalDelay(data, nData);
                break;
            case r_fine:
                continueLoop = 0;
                break;
            default:
                printf("Il comando inserito non %c valido. Riprovare.\n", 138);
        }
    } while(continueLoop);

    return 0;
}

command readCommand() {
    char string[MAX_COMMAND_LEN + 1];

    printf("\nRicorda: \n"
           "data: per elencare tutte le corse tra due date\n"
           "partenza: per elencare tutte le corse partite da una certa fermata\n"
           "capolinea: per elencare tutte le corse che hanno capolinea ad una certa fermata\n"
           "ritardo: per elencare tutte le corse che, in un certo intevallo di date, hanno accumulato ritardo\n"
           "ritardo_tot: per elencare il ritardo totale accumulato in una tratta"
           "fine: per concludere l'esecuzione\n\n");
    printf("Inserisci un comando: ");
    gets(string);
    if(strcmp(string, "data") == 0) {
        return r_date;
    } else if(strcmp(string, "partenza") == 0) {
        return r_partenza;
    } else if(strcmp(string, "capolinea") == 0) {
        return r_capolinea;
    } else if(strcmp(string, "ritardo") == 0) {
        return r_ritardo;
    } else if(strcmp(string, "ritardo_tot") == 0) {
        return r_ritardoTot;
    } else if(strcmp(string, "fine") == 0) {
        return r_fine;
    } else {
        return -1;
    }
}

/*
 * La funzione richiede le due date, inf formato AAAAMMGG per considerarle direttamente come numeri, e scandisce il
 * vettore delle corse per cercare quelle che hanno la data compresa tra quelle indicate
 * 
 * d[]: vettore delle corse salvate
 * nd:  numero effettivo delle corse salvate
 * 
 * return: -
 */
void dates(line d[MAX_DATA], int nd){
    char date[9];
    int d1, d2, i, cnt = 0;

    printf("Inserire la prima data(AAAAMMGG, senza spazi o /): ");
    gets(date);
    d1 = atoi(date);
    printf("Inserire la seconda data(AAAAMMGG, senza spazi o /): ");
    gets(date);
    d2 = atoi(date);

    if(d2 < d1) {
        //gestione dell'errore nell'inserimento delle date
        printf("La seconda data viene prima della prima, riprovare.\n");
        return;
    }

    printf("Ecco le corse comprese tra quelle due date: \n");
    for(i = 0; i < nd; i++) {
        if(atoi(d[i].date) >= d1 && atoi(d[i].date) <= d2) {
            printf("%s %s %s %s %s\n", d[i].code, d[i].startPoint, d[i].endPoint, d[i].startTime, d[i].endTime);
            cnt++;
        }
    }
    if(cnt == 0) {
        printf("Nessuna.\n");
    }
    return;
}

/*
 * La funzione richiede il punto di partenza e scandisce il vettore con le corse alla ricerca di dati con quel punto di
 * partenza
 * 
 * d[]: vettore delle corse salvate
 * nd:  numero effettivo delle corse salvate
 * 
 * return: -
 */
void departures(line d[MAX_DATA], int nd){
    char string[MAX_STR_LEN];
    int i, cnt;

    printf("Inserisci il punto di partenza di tuo interesse: ");
    gets(string);

    printf("Ecco le corse partite da quel punto:\n");
    for(i = 0; i < nd; i++) {
        if(strcmp(string, d[i].startPoint) == 0) {
            printf("%s %s %s %s %s\n", d[i].code, d[i].startPoint, d[i].endPoint, d[i].startTime, d[i].endTime);
            cnt++;
        }
    }
    if(cnt == 0) {
        printf("Nessuna.\n");
    }
}

/*
 * La funzione richiede il punto di arrivo e scandisce il vettore con le corse alla ricerca di dati con quel punto di
 * arrivo
 * 
 * d[]: vettore delle corse salvate
 * nd:  numero effettivo delle corse salvate
 * 
 * return: -
 */
void arrivals(line d[MAX_DATA], int nd){
    char string[MAX_STR_LEN];
    int i, cnt = 0;

    printf("Inserisci il punto di arrivo di tuo interesse: ");
    gets(string);

    printf("Ecco le corse arrivate in quel punto:\n");
    for(i = 0; i < nd; i++) {
        if(strcmp(string, d[i].endPoint) == 0) {
            printf("%s %s %s %s %s\n", d[i].code, d[i].startPoint, d[i].endPoint, d[i].startTime, d[i].endTime);
            cnt++;
        }
    }
    if(cnt == 0) {
        printf("Nessuna.\n");
    }
}

/*
 * La funzione richiede due date, in formato AAAAMMGG per considerarle direttamente come numeri, e scandisce il vettore
 * cercando le corse che tra quelle date hanno avuo un ritardo
 * 
 * d[]: vettore delle corse salvate
 * nd:  numero effettivo delle corse salvate
 * 
 * return: -
 */
void delay(line d[MAX_DATA], int nd){
    char date[9];
    int d1, d2, i, cnt = 0;

    printf("Inserire la prima data(aaaammgg, senza spazi o /): ");
    gets(date);
    d1 = atoi(date);
    printf("Inserire la seconda data(aaaammgg, senza spazi o /): ");
    gets(date);
    d2 = atoi(date);

    printf("Ecco le corse che sono arrivate in ritardo tra quelle due date: \n");
    for(i = 0; i < nd; i++) {
        if (atoi(d[i].date) >= d1 && atoi(d[i].date) <= d2) {
            printf("%s %s %s %s %s\n", d[i].code, d[i].startPoint, d[i].endPoint, d[i].startTime, d[i].endTime);
            cnt++;
        }
    }
    if(cnt == 0) {
        printf("Nessuna");
    }
}

/*
 * La funzione richiede un codice di tratta e va a scandire il vettore cercando le corse di quella tratta e andando a
 * sommare il loro ritardo in modo da conoscere il ritardo totale su quella tratta
 * 
 * d[]: vettore delle corse salvate
 * nd:  numero effettivo delle corse salvate
 * 
 * return: -
 */
void totalDelay(line d[MAX_DATA], int nd){
    char string[MAX_STR_LEN];
    int i, cntDelay = 0;

    printf("Inserisci il codice tratta di tuo interesse: ");
    gets(string);

    for(i = 0; i < nd; i++) {
        if(strcmp(string, d[i].code) == 0) {
            cntDelay+=d[i].delay;
        }
    }
    printf("Il ritardo totale accumulato su questa tratta %c: %d", 138, cntDelay);
}
