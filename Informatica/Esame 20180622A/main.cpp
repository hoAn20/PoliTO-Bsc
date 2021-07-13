#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int main (int argc, char *argv[]) {
	FILE *f, *g, *burton;	//<hh:mm:ss> <valore> int, int, int, float
	int h, m, s, h2, m2, s2;
	int tempo, tempos, tempop;
	float valore1, valore2, valorep  = -1;
	int stop;

	//controlli preliminari
	if (((f = fopen ("primo.txt", "r")) == NULL) || ((g = fopen ("secondo.txt", "r")) == NULL) || ((burton = fopen("burton.txt", "w")) == NULL)) {
		printf ("Errore nel caricamento dei file necessari.\n");
		exit (1);
	}

	//elaborazione del file
	fscanf (f, "%d:%d:%d %f", &h, &m, &s, &valore1);
	tempop = h * 3600 + m * 60 + s;
	fprintf (burton, "%d:%d:%d %.2f", h, m, s, valore1);
	while (!feof (f)) {
		fscanf (f, "%d:%d:%d %f", &h, &m, &s, &valore1);
		tempos = h * 3600 + m * 60 + s;
		if (!feof (f) && !feof (g)){
			printf ("4");
			stop = 0;
			if (valorep != -1)
				fprintf (burton, " %.2f", valorep);
			do {
				fscanf (g, "%d:%d:%d %f", &h2, &m2, &s2, &valore2);
				tempo = h2 * 3600 + m2 * 60 + s2;
				if (tempo >= tempop && tempo < tempos)
					fprintf (burton, " %.2f", valore2);
				else{
					valorep = valore2;
					stop = 1;
				}
				printf ("1");
			} while (!stop);
		} else if (feof (f)){
			printf ("3");
			fprintf (burton, "\n%d:%d:%d %.2f %.2f", h, m, s, valore1, valorep);
			while (!feof (g)) {
				fscanf (g, "%d:%d:%d %f", &h2, &m2, &s2, &valore2);
				fprintf (burton, " %.2f", valore2);
				printf ("2");
			}
		}
		if (!feof(f)){
			fprintf (burton, "\n%d:%d:%d %.2f", h, m, s, valore1);
		}
		tempop = tempos;
	}

	fclose (f);
	fclose (g);
	fclose (burton);
	return 0;
}
