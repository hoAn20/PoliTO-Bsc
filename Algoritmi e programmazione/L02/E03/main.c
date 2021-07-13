
//L02E03

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_SEQ_LEN = 5;
const int MAX_N_SEQ = 20;
const int MAX_ROW_LEN = 200;
const int MAX_WORD_LEN = 25;

void stringToLower(char s[]);
int foundSeq(char string[], char sequence[]);

int main() {
    FILE *fsequences, *ftext;
    char sequences[MAX_N_SEQ][MAX_SEQ_LEN], row[MAX_ROW_LEN], word[MAX_WORD_LEN];
    int nSequences, cntWord, i, j, found, k;

    if((fsequences = fopen("sequenze.txt", "r")) == NULL || (ftext = fopen("testo.txt", "r")) == NULL) {
        printf("Si %c verificato un errore nell'apertura di uno dei file, controllare che esistano e riporvare.\n", 138);
        return EXIT_FAILURE;
    }

    fscanf(fsequences,"%d", &nSequences);
    for (i = 0; i < nSequences; i++) {
        fscanf(fsequences, "%s", sequences[i]);
        stringToLower(sequences[i]);
    }

    //si scandisce il file delle sequenza e per ogni sequenza si va a ricercare nel file del testo (parola per parola)
    //una corrispondenza sfruttando la funzione foundSeq
    for(k = 0; k < nSequences; k++) {
        cntWord = 0;
        found = 0;
        printf("La sequenza %s %c contenuta in:\n", sequences[k], 138);
        while(fgets(row, MAX_ROW_LEN, ftext) != NULL) {
            j = 0;
            do {
                i = 0;
                //si compone ogni parola del testo leggendola carattere per carattere
                while (isalnum(row[j])) {
                    word[i] = row[j];
                    i++;
                    j++;
                }
                word[i] = '\0';
                cntWord++;
                if (foundSeq(word, sequences[k])) {
                    found = 1;
                    printf("%s: posizione %d\n", word, cntWord);
                }
                while(isspace(row[j]) || ispunct(row[j])) {
                    j++;
                }
            } while(row[j] != '\n' && row[j] != '\0');
        }
        rewind(ftext);
        if(!found) {
            printf("nessuna delle parole\n");
        }
        printf("\n");
    }

    return 0;
}

/*
 * La funzione prende una stringa e la restituisce scritta in soli caratteri minuscoli
 *
 * s:   stringa da rendere minuscola
 *
 * return:  -
 */
void stringToLower(char s[]) {
    int i;

    for(i  = 0; i < strlen(s); i++) {
        s[i] = tolower(s[i]);
    }
}

/*
 * La funzione scandisce carattere per carattere la parola estratta dal testo e cercando una corrispondenza con la
 * sequenza passata dal main
 *
 * string:      parola letta dal file di testo
 * sequence:    sequenza da cercare nella parola
 *
 * return:  se è stata trovata o meno una corrispondenza
 */
int foundSeq(char string[], char sequence[]) {
    int i,j, found = 0;

    for (i = 0; i < strlen(string); i++) {
        if(tolower(string[i]) == sequence[0]) {
            found = 1;
            for(j = 1; j < strlen(sequence); j++) {
                if(tolower(string[i + j]) != sequence[j]) {
                    found = 0;
                }
            }
        }
        if(found) {
            return 1;
        }
    }
    return 0;
}
