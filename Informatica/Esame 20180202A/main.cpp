#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[]) {
	int codice, matricola, uscita, h, m, s, tempi [1000] = {0}, accessi[1000], t, imax, imin, i;
	FILE *f;

	if (argc != 3) {
		printf ("Errore nei parametri.\n");
		exit (1);
	}
	if ((f = fopen (argv[1], "r")) == NULL) {
		printf ("Impossibile aprire il file specificato.\n");
		exit (1);
	}

	while (!feof (f)) {
		fscanf (f, "%d %d %d %d:%d:%d", &codice, &matricola, &uscita, &h, &m, &s);
		t = (h * 3600) + (m * 60) + s;
		if (!uscita) {
			accessi[matricola] = t; 
		} else {
			tempi[matricola] += (t - accessi[matricola]); 
		}
	}
	if (strcmp (argv[2], "-v") == 0) {
		for (i = 0; i < 1000; i++) {
			if (tempi[i]) {
				h = (int)tempi[i] / 3600;
				tempi[i] -= h * 3600;
				m = (int)tempi[i] / 60;
				tempi[i] -= m * 60;
				s = (int)tempi[i];
				printf ("mat: %d\t%d:%d:%d\n", i, h, m, s);
			}
		}
	} else if (strcmp (argv[2], "-c") == 0) {
		imax = matricola;
		imin = matricola;
		for (i = 1; i < 1000; i ++) {
			if (tempi[i] > tempi[imax]) {
				imax = i;
			}
			if (tempi[i] != 0 && tempi[i] < tempi[imin]) {
				imin = i;
			}
		}
		printf ("Maggior tempo: mat. %d\nMinor tempo: mat.%d\n", imax, imin);
	} else {
		printf ("Impossibile gestire la richiesta.\n");
		exit (1);
	}
	fclose (f);
	return 0;
}
