#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int main (int argc, char *argv[]) {
	char idgiocata[11];
	int numerigiocato, qtanumeri, i, presi, j;
	int e[6], numeri[4], vincitori = 0;
	FILE *f;
	
	for (i = 2; i < 8; i++) {
		e[i-1] = atoi(argv[i]);
	}

	if (argc != 8) {
		printf ("Errore nei parametri passati al programma.\n");
		exit (1);
	}
	if ((f = fopen (argv[1], "r")) == NULL) {
		printf ("Impossibile aprire il file specificato.\n");
		exit (1);
	}

	while (!feof (f)) {
		fscanf (f, "%s %d", idgiocata, &qtanumeri);
		presi = 0 ;
		for (i = 0; i < qtanumeri; i++) {
			fscanf (f, "%d", &numerigiocato);
			for (j = 0; j < 6; j++) {
				if (numerigiocato == e[j])
					presi++;
			}
		}

		if (presi == 3) 
			numeri[presi - 3]++;
		if (presi == 4)
			numeri[presi - 3]++;
		if (presi == 5)
			numeri[presi -3]++;
		if (presi == 6)
			numeri[presi - 3]++;
		if (presi == 3 || presi == 4 || presi == 5 || presi == 6) 
			printf ("%s %d numeri indovinati\n", idgiocata, presi);
	}
	printf ("\nTotale:\n");
	for (i = 0; i < 4; i++) {
		if (numeri[i])
			printf ("%d: %d vincitori\n", (i + 3), numeri[i]);
		else
			printf ("%d: nessun vincitore\n", (i + 3));
		vincitori += numeri[i];
	}
	printf ("\nTotale vincitori: %d", vincitori);
	fclose (f);
	return 0;
}
