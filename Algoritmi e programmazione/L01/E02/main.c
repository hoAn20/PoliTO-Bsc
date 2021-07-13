
//L01E02

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_ENCDICT_LEN = 30;
const int MAX_ROW_LEN = 200;
const int MAX_WORD_LEN = 36;

int findSectionToCode(char w[MAX_WORD_LEN], char d[MAX_ENCDICT_LEN][2][MAX_WORD_LEN + 1], int *e, int dLen);

int main() {
    FILE *f, *r;
    char row[MAX_ROW_LEN], encriptionDictionary[MAX_ENCDICT_LEN][2][MAX_WORD_LEN + 1], word[MAX_WORD_LEN + 1], spaceOrReturn;
    int i, encDictLen, position, encoder;

	//apertura del file dizionario
    if((f = fopen("dizionario.txt", "r")) == NULL) {
    	        //gestione di un errore nell'apetura del file
        printf("Impossibile aprire il file dizionario.txt, verificare che esista e riprovare.\n");
        return EXIT_FAILURE;
    }
    fscanf(f, "%d", &encDictLen);
    if(encDictLen > MAX_ENCDICT_LEN) {
    	//gestione errore nella lunghezza specificata da file
        printf("Il file dizionario %c troppo lungo.\n\n", 138);
        return EXIT_FAILURE;
    }
    for(i = 0; i < encDictLen; i++) {
        fscanf(f, "%s %s", encriptionDictionary[i][0], encriptionDictionary[i][1]);
    }
    fclose(f);

    if((f = fopen("sorgente.txt", "r")) == NULL) {
        printf("Impossibile aprire il file sorgente.txt, verificare che esista e riprovare.\n\n");
        return EXIT_FAILURE;
    }
    r = fopen("ricodificato.txt", "w");

	//lettura del file parola per parola
    while(!feof(f)) {
    	//oltre alla singola parola vado a leggere il carattere successivo che tanto sarà o uno spazio o un a capo,
        //in questo modo so se andare a capo o lasciare uno spazio durante la ricodifica
        fscanf(f, "%s%c", word, &spaceOrReturn);
        position = findSectionToCode(word, encriptionDictionary, &encoder, encDictLen);
        if (position != -1) {//il valore -1 viene ritornato nel caso non si trovi alcuna corrispondenza, è stato
                                //scelto perché non è un possibile indice

            //stampa le lettere della parola letta fino alla posizione in cui inizia la corrispondenza con il codice cifrato
            for (i = 0; i < position; i++){
                fprintf(r, "%c", word[i]);
            }
            //stampa l'intera parte del codice cifrato
            fprintf(r, "%s", encriptionDictionary[encoder][0]);
            //stampa la parte restante della parola letta
            for (i = position + strlen(encriptionDictionary[encoder][1]); i < strlen(word); i++) {
                fprintf(r, "%c", word[i]);
            }
        } else {
        	//nel caso in cui non sia stata trovata alcuna corrispindenza con il dizionario stampa l'intera parola
            fprintf(r, "%s", word);
        }
        //stampa infine il carattere che seguiva la parola, andando quindi a capo come andava il testo sorgente
        fprintf(r, "%c", spaceOrReturn);
    }
    printf("Il testo %c stato ricodificato seguendo il dizionario fornito.", 138);
    fclose(f);
    fclose(r);

    return 0;
}

/*
 * La fuzione scandisce la matrice contenente il dizionario andando a cercare nella prola letta una corrispondenza
 * dell'iniziale, se la trova inizia a scandire le successive lettere della parola per controllare che la sequenza da
 * cifrare sia effettivamente contenuta
 *
 * w:   parola in cui cercare una corrispondenza
 * d:   dizionario con le sequenza da cifrare
 * e: 	indice della corrispondente cifratura
 * dLen: lunghezza del dizionario
 *
 * return: in caso di corrispondeza l'inidice da cui inizia, altrimenti -1
 */
int findSectionToCode(char w[MAX_WORD_LEN], char d[MAX_ENCDICT_LEN][2][MAX_WORD_LEN + 1], int *e, int dLen) {
    int i, k, found, j;

    for(i = 0; i < strlen(w); i++){
        for(j = 0; j < dLen; j++) {
            if(w[i] == d[j][1][0]) {
                found = 1;
                for( k = 1; k < strlen(d[j][1]); k++) {
                    if(w[i + k] != d[j][1][k]) {
                        found = 0;
                    }
                }
                if(found) {
                    *e = j;
                    return i;
                }
            }
        }
    }

    return -1;
}
