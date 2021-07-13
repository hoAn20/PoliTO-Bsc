#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
	FILE *f, *g;	//file misure.txt e limiti.txt
	float p[5][10], a[10], l[10][2];
	float media;
	int anomalie = 0, riga;
	int anomalieMax = atoi (argv[1]);
	int i;

	if (argc != 2) {
		printf ("Errore nei parametri.\n");
		exit (1);
	}
	if ((f = fopen ("misure.txt", "r")) == NULL || (g = fopen ("limiti.txt", "r")) == NULL) {
		printf ("Impossibile trovare i file necessari.\n");
		exit (1);
	}

	for (i = 0; i < 10; i++) {
		fscanf (g, "%f %f", &l[i][0], &l[i][1]);
	}
	for (riga = 0; riga < 5; riga++) {
		for (i = 0; i < 10; i++) {
			fscanf (f, "%f", &p[riga][i]);
		}
	}
	for (i = 0; i < 10; i++) {
		media = (p[0][i] + p[1][i] + p[2][i] + p[3][i] + p[4][i]) / 5;
		if (!((media > l[i][0]) && (media < l[i][1])))
			anomalie++;
	}
	if (anomalie > anomalieMax)
		printf ("Anomalia alla riga: %d\n", (riga + 1));
	while (!feof (f)) {
		for (i = 0; i < 10; i++) {
			fscanf (f, "%f", &a[i]);
		}
		riga ++;
		anomalie = 0;
		for (i = 0; i < 10; i++) {
			p[0][i] = p[1][i];
			p[1][i] = p[2][i];
			p[2][i] = p[3][i];
			p[3][i] = p[4][i];
			p[4][i] = a[i];
			media = (p[0][i] + p[1][i] + p[2][i] + p[3][i] + p[4][i]) / 5;
			if (!((media > l[i][0]) && (media < l[i][1])))
				anomalie++;
		}
			if (anomalie > anomalieMax)
		printf ("Anomalia alla riga: %d\n", (riga + 1));
	}
	fclose (f);
	fclose (g);
}
