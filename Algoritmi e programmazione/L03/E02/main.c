#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int sign;
    int exponent;
    int significand;
} DATA_TYPE;

int isBigEndian();
void printCode(void *p, int size, int bE, DATA_TYPE dT);

int main() {
    float af;
    long double ald;
    double ad;
    int bigEndian = isBigEndian();
    DATA_TYPE dataType[3];

    printf("Inserire un float: ");
    scanf("%f", &af);
    printf("\nIl double e il long double verranno ottenuti tramite cast.\n\n");

    //cat del dato float in double o long double e assegnazione delle dimensioni in bit di segno, esponente e mantissa
    dataType[0].sign = 1;
    dataType[0].exponent = 8;
    dataType[0].significand = 23;
    ad = (double)af;
    dataType[1].sign = 1;
    dataType[1].exponent = 11;
    dataType[1].significand = 52;
    ald = (long double)af;
    dataType[2].sign = 1;
    dataType[2].exponent = 15;
    dataType[2].significand = 64;

    //specifico qale sia il tipo di codifica interna
    printf("Tipo di codifica: ");
    if (bigEndian) {
        printf("BigEndian\n\n");
    } else {
        printf("LittleEndian\n\n");
    }

    //scrivo la lunghezza in byte e in bit di tutti e tre i tipi di dato
    printf("La variabile float %c lunga %d byte (%d bit)\n", 138, sizeof(af), 8*sizeof(af));
    printf("La variabile double %c lunga %d byte (%d bit)\n", 138, sizeof(ad), 8*sizeof(ad));
    printf("La variabile long double %c lunga %d byte (%d bit)\n\n", 138, sizeof(ald), 8*sizeof(ald));

    printf("Float:\n");
    printCode((void*)&af, sizeof(af), bigEndian, dataType[0]);
    printf("Double:\n");
    printCode((void*)&ad, sizeof(ad), bigEndian, dataType[1]);
    printf("Long Double:\n");
    printCode((void*)&ald, sizeof(ald), bigEndian, dataType[2]);

    system("pause");
    return 0;
}

/*
 * La funzione assegna ad una variabile il valore 1 che è rappresentato da 31 bit a 0 e un bit a 1, va a leggere il
 * primo byte assegnando ad un puntatore char (che ha dimensione 1 byte) l'indirizzo della variabile che va vale 1. Nel
 * caso in cui il valore del inter del byte estratto sia 1 significa che la codifica è di tipo littleEndian altrimenti
 * di tipo bigEndian
 *
 * return: 1 se è bigEndian, 0 se è littleEndian
 */
int isBigEndian() {
    int n = 1;
    char *firstByte = (char*)&n;
    return (*firstByte == 0);
}

/*
 * La funzione controlla il valore assunto dalla variabile bE e in base ad esso va ad assegnare ai puntatori in bytes
 * le corrette aree di memoria di p, nel caos in cui sia littleEndian va ad assegnarli al contrario. Procede andando a
 * considerare il valore intero assunto dalle aree di memoria puntate dai puntatori in bytes e ne effettua la codifica
 * binaria andando a sistemare gli 1 e 0 già nell'ordine in cui dovranno essere stampati. Conclude andando a stampare la
 * codifica divisa per segno, esponente e mantissa basandosi sulla lunghezza specificata all'interno della struttura dT
 *
 * p:   puntatore all'area di memoria in cui è salvato il dato da scrivere in binario
 * size: dimensione in byte del dato memorizzato in quell'area di memoria
 * bE:  flag per indicare se la codifica è di tipo biEndian o littleEndian
 * dT: struttura contenente le lunghezze di segno, mantissa e esponente
 *
 * return: -
 */
void printCode(void *p, int size, int bE, DATA_TYPE dT) {
    unsigned char *bytes[16];
    int code[16*8];
    int j, i, start = 0;

    /*
     * Per testare l'altro tipo di codifica si può introdurre una secondo vettore di puntatori a char ed assegnare a
     * questo i byte nell'oridne opposto a quello con cui apaiono in p e modificare le assegnazioni all'interno del
     * seguente if in modo da assegnare ai puntatori in bytes le aree puntate dal secondo vettore di puntatori
     */

    if (bE) {
        for (i = 0; i < size; i++) {
            bytes[i] = (unsigned char*)(p+i);
        }
    } else {
        for (i = 0; i < size; i++) {
            bytes[i] = (unsigned char*)(p + (size - (i + 1)));
        }
    }

    for(i = 0; i < size; i++) {
        for (j = 8*(i+1) - 1; (8 * (i + 1) - 1 - j) < 8; j--) {
            code[j] = *bytes[i] % 2;
            *bytes[i] = *bytes[i]/2;
        }
    }

    if (size == sizeof(long double)) {
        start = 8*size - (dT.sign + dT.exponent + dT.significand);
    }

    printf("Segno(%d bit): %d\n", dT.sign, code[start]);

    i = start + 1;
    printf("Esponente(%d bit): ", dT.exponent);
    while(i - start <= dT.exponent){
        printf("%d", code[i]);
        i++;
    }
    printf("\n");
    printf("Mantissa(%d bit): ", dT.significand);
    while(i - start - dT.exponent <= dT.significand){
        printf("%d", code[i]);
        i++;
    }
    printf("\n");

    printf("\n");

    return;
}