
//L00E01

#include <stdio.h>
#include <stdlib.h>

const int N = 10;

int isProperSubVect(int *v, int maxLength, int j);

int main() {
    int vect[N];
    int maxSubVectLength = 0, subVectLength = 0;
    int i, j;

    for(i = 0; i < N; i++) {
        printf("Inserisci il numero della cella '%d' del vettore: ", i);
        scanf("%d", &vect[i]);

        //cerco la maxSubVectLenght già durante la fase di input dei dati
        if(vect[i] != 0) {
            subVectLength++;
        } else {
            if(subVectLength > maxSubVectLength) {
                maxSubVectLength = subVectLength;
            }
            subVectLength = 0;
        }
    }
    system("cls");

    printf("I sotto-vettori di lunghezza massima (%d) sono:\n", maxSubVectLength);
    for(i = 0; i < N - maxSubVectLength; i++) {
        if(isProperSubVect(vect, maxSubVectLength, i)) {
            printf("[ ");
            for(j = i; (j - i) < maxSubVectLength; j++) {
                printf("%d ", vect[j]);
            }
            printf("]\n");
        }
    }
    system("pause");

    return 0;
}

int isProperSubVect(int *v, int maxLength, int j) {
    int isMaxLength = 1;
    int i;

    for(i = j; i < j + maxLength; i++) {
        if(v[i] == 0) {
             isMaxLength = 0;
        }
    }

    return isMaxLength;
}
