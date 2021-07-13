#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define N 4
#define M 2

int main (int argc, char *argv[]) {
	int k = atoi (argv[2]);
	int v[N * M], val;
	int escursione, intervallo, nintervalli, max, min, dimeff = 0, i, j, uguale, si;
	FILE *f;

	if (argc != 3) {
		printf ("Errore nei parametri.\n");
		exit (1);
	}
	if ((f = fopen (argv[1], "r")) == NULL) {
		printf ("Impossibile aprire il file specificato.\n");
		exit (1);
	}

	for (i = 0; i < N * M; i++) {
		fscanf (f, "%d", &val);
		uguale = 0;
		for (j = 0; j < dimeff; j++){
			if (val == v[j])
				uguale = 1;
		}
		if (!uguale) {
			v[dimeff] = val;
			dimeff++;
		}
	}
	fclose (f);
	max = v[dimeff - 1];
	min = v[0];
	escursione = max - min;
	intervallo = (escursione / k) + 1;
	nintervalli = escursione / intervallo;
	for (i = 0; i < nintervalli + 1; i++) {
		printf ("%d intervallo: ", (i + 1));
		si = 0;
		for (j = 0; j < dimeff; j ++) {
			if ((min + (i * intervallo) )<= v[j] && v[j] < (min + (i + 1) * intervallo)){
				printf ("%d\t", v[j]);
				si = 1;
			}
		}
		if (si)
			printf ("\n");
		else
			printf ("nessuno\n");
	}
	return 0;
}
