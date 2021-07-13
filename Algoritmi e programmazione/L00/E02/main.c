
//L00E02

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_STR_LEN = 20;

int isVowel(char c);
int count(char S[MAX_STR_LEN], int n);

int main() {
    FILE *f;
    char string[MAX_STR_LEN], fileName[MAX_STR_LEN];
    int fileLength, subStringLength, total = 0;
    int i;

    printf("Inserisci il nome del file da cui prendere le parole: ");
    gets(fileName);
    printf("Inserisci la lunghezza delle sottostringhe su cui lavorare: ");
    scanf("%d", &subStringLength);

    if((f = fopen(fileName, "r")) == NULL) {
        printf("Non %c risulta possibile aprire il file specificato. Riprovare.\n", 138);
        system("pause");
        return EXIT_FAILURE;
    }

    fscanf(f, "%d", &fileLength);

    for(i = 0; i < fileLength; i++) {
        fscanf(f, "%s", string);
        total += count(string, subStringLength);
    }

    printf("\nIl numero di sotto-stringhe contenenti 2 vocali %c: %d\n", 138, total);
    system("pause");

    return 0;
}

int count(char S[MAX_STR_LEN], int n) {
    int counter = 0, vowelCounter;
    int i, j;

    for (i = 0; i < (strlen(S) - n + 1); i++) {
        vowelCounter = 0;
        for(j = i; (j - i) < n; j++) {
            if(isVowel(S[j])) {
                vowelCounter++;
            }
        }
        if(vowelCounter == 2) {
            counter++;
        }
    }

    return counter;
}

int isVowel(char c) {
    if(c == 'A' || c == 'a' || c == 'E' || c == 'e' || c == 'I' || c == 'i' || c == 'O' || c == 'o' || c == 'U' || c == 'u') {
        return 1;
    } else {
        return 0;
    }
}
