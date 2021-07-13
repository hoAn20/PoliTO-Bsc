#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 50

typedef enum {acq_ts, acq_file, ric_cod, canc_cod, canc_date, st_file, st_video, fine} comando;

typedef struct {
    int giorno;
    int mese;
    int anno;
} data;
typedef struct {
    char via[MAX_STR_LEN+1];
    char citta[MAX_STR_LEN+1];
    int CAP;
} indirizzo;
typedef struct {
    char chiave[7];
    char cognome[MAX_STR_LEN+1];
    char nome[MAX_STR_LEN+1];
    data dataNascita;
    indirizzo residenza;
} ITEM;
typedef struct node *LINK, NODO;
struct node {
    ITEM val;
    LINK next;
};

comando leggiComando();
char *estrazioneCHIAVE(ITEM *item);
int estrazioneDATA(ITEM item);
int DATA_Meq(int data1, int data2); //data minore corrisponde a persona più anziana
int DATA_meq(int data1, int data2);
int CHIAVE_eq(char *chiave1, char *chiave2);
int dataToInt(data d1);
ITEM ITEMLeggiNuovo(FILE *f);
ITEM LISTARicerca(LINK h, char *chiave);
ITEM ITEMNuovoVuoto();
ITEM LISTACancellaEstrazioneCodice(LINK *hp, char *chiave);
LINK NODOnuovo(ITEM val, LINK next);
LINK LISTAInserimentoOrdinato(LINK h, ITEM val);
LINK CancellaIntervalloDate(LINK h, data data1, data data2);
LINK LISTAEstrazioneData(LINK h);
void LISTAStampa(FILE *f, LINK h);
void ITEMStampa(ITEM item);



int main() {
    LINK head = NULL;
    comando comandoRicevuto;
    FILE *fInput, *fOutput;
    char nomeFile[MAX_STR_LEN+1], codiceCercato[MAX_STR_LEN+1];
    ITEM item;
    data data1, data2;

    do {
        comandoRicevuto = leggiComando();

        switch (comandoRicevuto) {
            case acq_ts:
                fInput = stdin;
                head = LISTAInserimentoOrdinato(head, ITEMLeggiNuovo(fInput));
                break;
            case acq_file:
                printf("Inserisci il nome del file da cui vuoi leggere: ");
                scanf("%s", nomeFile);
                fInput = fopen(nomeFile, "r");
                if (fInput == NULL)
                    printf("Impossibile leggere il file specificato.\n");
                else {
                    while(!feof(fInput))
                        head = LISTAInserimentoOrdinato(head, ITEMLeggiNuovo(fInput));
                }
                fclose(fInput);
                break;
            case ric_cod:
                printf("Inserisci il codice da ricercare: ");
                scanf("%s", codiceCercato);
                item = LISTARicerca(head, codiceCercato);
                if (strcmp(item.chiave, "0000000") == 0)
                    printf("Elemento non trovato.\n");
                else {
                    printf("- Elemento corrispondente -\n");
                    ITEMStampa(item);
                }
                break;
            case canc_cod:
                printf("Inserisci il codice dell'elemento che vuoi cancellare: ");
                scanf("%s", codiceCercato);
                item = LISTACancellaEstrazioneCodice(&head, codiceCercato);
                if (strcmp(item.chiave, "0000000") == 0)
                    printf("Elemento non trovato.");
                else {
                    printf(" - Elemento cancellato -\n");
                    ITEMStampa(item);
                }
                break;
            case canc_date:
                printf("Inserisci la prima data(gg/mm/aaaa): ");
                scanf("%d/%d/%d", &data1.giorno, &data1.mese, &data1.anno);
                printf("Inserisci la seconda data(gg/mm/aaaa): ");
                scanf("%d/%d/%d", &data2.giorno, &data2.mese, &data2.anno);
                head = CancellaIntervalloDate(head, data1, data2);
                break;
            case st_file:
                printf("Inserisci il nome del file su cui vuoi scrivere: ");
                scanf("%s", nomeFile);
                fOutput = fopen(nomeFile, "w");
                LISTAStampa(fOutput, head);
                fclose(fOutput);
                break;
            case st_video:
                fOutput = stdout;
                LISTAStampa(fOutput, head);
                break;
            case fine:
                printf("Grazie per aver usato questo programma.");
                break;
            default:
                printf("Inserisci un comando valido.");
        }
    } while(comandoRicevuto!= fine);
}



/*
Funzione che legge una stringa di comando e ritorna un rispettivo valore di un enum in odo che il man possa far eseguire la corretta operazione
in base a quanto richiesto dall'utente
*/
comando leggiComando() {
    char riga[MAX_STR_LEN];

    fflush(stdin);
    printf("\n\n - MENU -\n"
           "acquisisci -t/-f : acquisiszione di dati da tastiera/ da file\n"
           "ricerca -c : ricerca per codice\n"
           "cancella -c/-d : cancellazione per codice/per intevallo di date\n"
           "stampa -f/-v : stampa su file\n"
           "fine : per concludere il programma\n\n"
           "Inserisci il comando: ");
    gets(riga);
    fflush(stdin);

    if (strcmp(riga, "acquisisci -t") == 0)
        return acq_ts;
    else if (strcmp(riga, "acquisisci -f") == 0)
        return acq_file;
    else if (strcmp(riga, "ricerca -c") == 0)
        return ric_cod;
    else if (strcmp(riga, "cancella -c") == 0)
        return canc_cod;
    else if (strcmp(riga, "cancella -d") == 0)
        return canc_date;
    else if (strcmp(riga, "stampa -f") == 0)
        return st_file;
    else if (strcmp(riga, "stampa -v") == 0)
        return st_video;
    else if (strcmp(riga, "fine") == 0)
        return fine;
    else
        return -1;
}
/*
Funzione di estrazione della chiave da un item
*/
char *estrazioneCHIAVE(ITEM *item) {
    return (*item).chiave;
}
/*
Funzione di estrazione della data trasformata in dato intero per facilitarne il confronto
*/
int estrazioneDATA(ITEM item) {
    return dataToInt(item.dataNascita);
}
/*
Funzione che definisce un confronto di Maggiore o uguale tra due date
*/
int DATA_Meq(int data1, int data2) {
    return (data1 >= data2);
}
/*
Funzione che definisce un confronto di minore o uguale tra due date
*/
int DATA_meq(int data1, int data2) {
    return (data1 <= data2);
}
/*
Funzione che restituisce un confronto di uguaglianza tra due chiavi estratte da due item
*/
int CHIAVE_eq(char *chiave1, char *chiave2) {
    return (strcmp(chiave1, chiave2) == 0);
}
/*
Funzione che passato un valore data restituisce il corrispettivo valore intero in formato amg in modo da facilitarne il confronto
*/
int dataToInt(data d) {
    return 10000*d.anno + 100*d.mese + d.giorno;
}
/*
FUnzion che permette di acquisisre un nuovo item attraverso la lettura da un file (quindi anche stdin)
*/
ITEM ITEMLeggiNuovo(FILE *f) {
    ITEM item;
    FILE *fout;

    if (f == stdin)
        fout = stdout;
    else
        fout = NULL;

    fprintf(fout, " - NUOVO ELEMENTO -\n"
           "Inserisci il CODICE: ");
    fscanf(f, "%s", item.chiave);
    fprintf(fout, "Inserisci il COGNOME: ");
    fscanf(f, "%s", item.cognome);
    fprintf(fout, "Inserisci il NOME: ");
    fscanf(f, "%s", item.nome);
    fprintf(fout, "Inserisci la data di nascita (gg/mm/aaaa): ");
    fscanf(f, "%d/%d/%d", &item.dataNascita.giorno, &item.dataNascita.mese, &item.dataNascita.anno);
    fprintf(fout, " - Indirizzo residenza -\n"
           "Inserisci la via: ");
    fscanf(f, "%s", item.residenza.via);
    fprintf(fout, "Inserisci la citt%c: ", 133);
    fscanf(f, "%s", item.residenza.citta);
    fprintf(fout, "Inserisci il CAP: ");
    fscanf(f, "%d", &item.residenza.CAP);

    return item;
}
/*
Funzione che implementa la ricerca per chiave e ritorna l'item trovato o un item con una chiava 000000 se non è stato trovato
*/
ITEM LISTARicerca(LINK h, char *chiave) {
    LINK x;

    for (x = h; x!= NULL; x = x->next) {
        if (CHIAVE_eq(estrazioneCHIAVE(&(x->val)), chiave))
            return x->val;
    }
    return ITEMNuovoVuoto();
}
/*
Funzionce che genera un item non valido con chiave 000000
*/
ITEM ITEMNuovoVuoto() {
    ITEM x;
    strcpy(x.chiave, "0000000");
    return x;
}
/*
Funzione che implementa la cancellazione per estrazione di un elemento data la chiave
*/
ITEM LISTACancellaEstrazioneCodice(LINK *hp, char *chiave) {
    LINK *xp, t;
    ITEM item = ITEMNuovoVuoto();
    for (xp = hp; (*xp)!= NULL; xp = &((*xp)->next)) {
        if (CHIAVE_eq(estrazioneCHIAVE(&((*xp)->val)), chiave)) {
            t = *xp;
            *xp = (*xp)->next;
            item = t->val;
            free(t);
            break;
        }
    }
    return item;
}
/*
FUnzion che va a generare un nuovo nodo dato l'item ce deve contenere e il collegamento al suceessivo
*/
LINK NODOnuovo(ITEM val, LINK next) {
    LINK x = malloc(sizeof *x);

    if(x == NULL)
        return NULL;
    else {
        x->val = val;
        x->next = next;
    }
    return x;
}
/*
Funzione che implementa l'inserimento ordinato di un elmento all'interno di una lista che potrebbe eanche essere vuota
*/
LINK LISTAInserimentoOrdinato(LINK h, ITEM val) {
    LINK x, p;

    int data = estrazioneDATA(val);
    if (h == NULL || DATA_Meq(data, estrazioneDATA(h->val)))
        return NODOnuovo(val, h);

    for (x = h->next, p = h; x != NULL && DATA_Meq(estrazioneDATA(x->val), data); p = x, x = x->next);
    p->next = NODOnuovo(val, x);
    return h;
}
/*
Funzione per l'estrazione di più item le cui dae di nascita siano comprese tra quelle specificate
*/
LINK CancellaIntervalloDate(LINK h, data data1, data data2) {
    int d1 = dataToInt(data1), d2 = dataToInt(data2), cnt = 0;
    LINK x;
    NODO nodoZero;

    if (h == NULL)
        return NULL;
    nodoZero.next = h;
    x = &nodoZero;
    printf(" - Elementi eliminati -\n");
    for (; x->next != NULL && x->next->next!= NULL;) {
        if (DATA_meq(estrazioneDATA(x->next->val), d2) && DATA_Meq(estrazioneDATA(x->next->val), d1)) {
            ITEMStampa(x->next->val);
            x = LISTAEstrazioneData(x);
            cnt++;
        } else if (DATA_meq(estrazioneDATA(x->next->val), d1))
            break;
        else
            x = x->next;
    }
    if (cnt == 0)
        printf("Nessun elemento cancellato.");
    h = nodoZero.next;
    return h;
}
/*
Funzione che si occupa dell'effettiva cancellazione di un item
*/
LINK LISTAEstrazioneData(LINK h) {
    LINK t = h->next;
    h->next = t->next;
    free(t);
    return h;
}
/*
Funzione che si occupa della stampa dell'intera lista su un file (che può anche essere stdout)
*/
void LISTAStampa(FILE *f, LINK h) {
    LINK x;
    for(x = h; x!= NULL; x = x->next) {
        fprintf(f, "%s %s %s %d/%d/%d %s %s %d\n", x->val.chiave, x->val.cognome, x->val.nome, x->val.dataNascita.giorno, x->val.dataNascita.mese, x->val.dataNascita.anno, x->val.residenza.via, x->val.residenza.citta, x->val.residenza.CAP);
    }
}
/*
Funzione che si occpa della stampa di un item 
*/
void ITEMStampa(ITEM item) {
    printf("%s %s %s %d/%d/%d %s %s %d\n", item.chiave, item.cognome, item.nome, item.dataNascita.giorno, item.dataNascita.mese, item.dataNascita.anno, item.residenza.via, item.residenza.citta, item.residenza.CAP);
}