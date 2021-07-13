#include <stdio.h>
#include <stdlib.h>

#define N 11
#define M 4

int main (int argc, char *argv[]) {
	FILE *f;
	int i, j, h, su, giu, dx, sx, continua, x, y;
	int nricerca, lunghezza, lricerca, ltotale = 0;
	float lmedia;
	char mappa[N][N];

	//controllo parametri
	if (argc != 2) {
		printf ("Errore nei parametri.\n");
		exit (1);
	}
	if ((f = fopen ("bath.txt", "r")) == NULL) {
		printf ("IMpossibile aprire i file necessari.\n");
		exit (1);
	}
	nricerca = atoi (argv[1]);
	if (nricerca < 1 || nricerca > M) {
		printf ("Il percorso cercato non risulta ricercabile.\n");
		exit (1);
	}

	//eleaborazione file
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			fscanf (f, "%c", &mappa[i][j]);
		}
	}

	//algoritmo principale
	for (h = 1; h < M + 1; h++) {
		for (i = 0; i< N; i++) {
			for (j = 0; j < N; j++) {
				if ((int)mappa[i][j] == h + 48) {
					x = i;
					y = j;
					lunghezza = 0;
					continua = 1;
					su = 0;
					giu = 0;
					dx = 0;
					sx = 0;
					while (continua) {
						if (x + 1 < N && !sx && mappa[x + 1][y] == '+') {
							x++;
							dx = 1;
							lunghezza++;
						} else {
							dx = 0;
							continua = 0;
						}
						if (!continua && x - 1 > 0 && !dx && mappa[x - 1][y] == '+') {
							x--;
							sx = 1;
							lunghezza ++;
							continua = 1;
						} else {
							sx = 0;
						}
						if (!continua && y + 1 < N && !su && mappa[x][y + 1] == '+') {
							y++;
							giu = 1;
							lunghezza++;
							continua = 1;
						} else {
							giu = 0;
						}
						if (!continua && y - 1 > 0 && !giu && mappa[x][y - 1] == '+') {
							y--;
							su = 1;
							lunghezza++;
							continua = 1;
						} else {
							su = 0;
						}
					}
					if (h == nricerca) {
						lricerca = lunghezza;
					}
					ltotale += lunghezza;
				}
			}
		}
	}
	lmedia = (float)ltotale / M;

	printf ("Il percorso %d %c lungo %d.\n", nricerca, 138, lricerca);
	printf ("La lunghezza media dei percorsi %c %.2f.\n", 138, lmedia);

	fclose (f);
	return 0;
}
