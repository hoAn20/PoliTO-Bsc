
//L04E01

#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b);

int main() {
    int a, b, MCD;

    printf("Inserisci il primo numero: ");
    scanf("%d", &a);
    printf("Inserisci il secondo numero: ");
    scanf("%d", &b);

    MCD = gcd(a, b);
    printf("Il gcd %c: %d\n\n", 138, MCD);
    return 0;
}

/*
La funzione gcd calcola il massimo comun divisore dei due numeri a e b, nel caso b sia maggiore di a li inverte prima di procedere secondo la definizione fornita
nel caso sinao uguai ne ritorna uno dei due, nel caso siano entrambi pari procede ricorsivamente con 2*gcd(a/2, b/2), nel caso a sia pari e b sia dispari procede
ricorsivamente con gcd(a/2, b), nel caso a sia dispari e b pari con gcd(a, b/2) altrimenti con gcd((a-b)/2, b).

a : primo numero
b : secondo numero
*/
int gcd(int a, int b) {
    int tmp;

    if (b > a) {
        tmp = a;
        a = b;
        b = tmp;
    }

    if (a == b)
        return a;

    if (a%2 == 0 && b%2 == 0)
        return (2*gcd(a/2, b/2));
    else if (a%2 == 0 && b%2 != 0)
        return gcd(a/2, b);
    else if (a%2 != 0 && b%2 == 0)
        return gcd(a, b/2);
    else
        return gcd((a-b)/2, b);
}