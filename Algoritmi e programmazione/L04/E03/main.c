
//L04E03

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_STR_LEN = 20;

int lunghezzaregexp(char *regexp);
char *cercaRegexp (char *src, char *regexp, int l);

int main() {
    char regexp[MAX_STR_LEN], row[MAX_STR_LEN], *pos;
    int i, trovata = 0, l;

    printf("Inserisci una stringa: ");
    gets(row);
    fflush(stdin);
    printf("\nRicorda:\n"
           ". accetta qualsiasi carattere\n"
           "[] accetta una qualsiasi delle lettere presenti tra le parentesi\n"
           "[^] accetta una qualsiasi lettera non presente tra le parentesi\n"
           "\\a accetta qualsiasi carattere minuscolo\n"
           "\\A accetta qualsiasi carattere maiuscolo\n\n"
           "Inserisci una regexp: ");
    gets(regexp);

    l = lunghezzaregexp(regexp);
    for (i = 0; i < strlen(row) - l + 1; i++) {
        pos = cercaRegexp((row + i), regexp, l);
        if (pos != NULL) {
            trovata = 1;
            printf("Occorrenza in posizione: %d\n\n", i);
        }
    }
    if (!trovata) {
        printf("Nessuna occorrenza.\n");
    }

    return 0;
}

/*
La funzione lunghezzaregexp calcola la lunghezza effettiva della stringa parametrizzata dalla regexp andando a saltare i metacaratteri ed
attribuendo loro la giusa lunghezza effettiva.

regexp : stringa contenente la regexp
*/
int lunghezzaregexp(char *regexp) {
    int l = 0;
    for (int i = 0; i < strlen(regexp); i++) {
        l++;
        if (regexp[i] == '[') {
            i+= 2;
            while (regexp[i] != ']')
                i++;
        } else if (regexp[i] == '\\')
            i++;
    }

    return l;
}

/*
La funzione cercaRegexp scandisce la stringa sorgente e in parallelo la stringa regexp in modo da controllare in funzione del metacarattere
se vi è una corrispondenza tra le due. La stringa sorgente parte da ogni posizione fino alla strlen(src)-l -esima in modo da controllare in
ogni possibile posizione se inizia una stringa del tipo descritto da regexp.
src : stringa in cui cercare la regexp, già traslata nel main di i caratteri
regexp : stringa contenente la regexp
l : lunghezza effettiva della regexp
*/
char *cercaRegexp (char *src, char *regexp, int l) {
    int i, j, k, trovata;

    for (i = 0, k = 0; i < strlen(regexp); i++, k++) {
        if (regexp[i] == '[' && regexp[i+1] != '^') {
            i++;
            trovata = 0;
            for (j = 0; regexp[i+j] != ']'; j++) {
                if (regexp[i+j] == src[k])
                    trovata = 1;
            }
            if (!trovata) {
                return NULL;
            }
            i+= j;
        } else if (regexp[i] == '[' && regexp[i+1] == '^'){
            i+= 2;
            for (j = 0; regexp[i+j] != ']'; j++) {
                if (regexp[i+j] == src[k])
                    return NULL;
            }
            i+= j;
        } else if (regexp[i] == '\\') {
            i++;
            if (isupper(regexp[i])) {
                if (!isupper(src[k]))
                    return NULL;
            } else {
                if (!islower(src[k]))
                    return NULL;
            }
        }else if (regexp[i] != src[k]){
            if (regexp[i] != '.')
                return NULL;
        }
    }

    return src;
}