#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int main (int argc, char *argv[]) {
	char prima[1000][21], seconda[40][101], decifrato[1001], stringa[21];
	FILE *p, *s, *cifrato;
	int i, trovato, plen, slen, j, h = 0;

	if (argc != 2) {
		printf ("Errore nei parametri inseriti.\n");
		exit (1);
	}
	if (((p = fopen("prima.txt", "r")) == NULL) || ((s = fopen("seconda.txt", "r")) == NULL) || ((cifrato = fopen(argv[1], "r")) == NULL)) {
		printf ("Errore nell'apertura di un file.\n");
		exit (1);
	}

	i = 0;
	while (!feof (p)) {
		fgets(prima[i], 21, p);
		i++;
	}
	fclose (p);
	plen = i;
	i = 0;
	while (!feof (s)) {
		fgets(seconda[i], 100, s);
		i++;
	}
	fclose (s);
	slen = i;
	while (!feof(cifrato)) {
		fgets (stringa, 21, cifrato);
		trovato = 0;
		i = -1;
		do {
			i++;
			if (strcmp(stringa, prima[i]) == 0){
				trovato = 1;
			}
		} while (!trovato);
		printf ("%d", i);
		/*j = 0;
		if (i > 101) {
			while (i > 101){
				i -= 101;
				j++;
			}
		}*/
		decifrato[h] = seconda[0][i];
		h++;
	}
	fclose (cifrato);
	for (i = 0; i< h; i++) {
		printf ("%c", decifrato[i]);
	}

	return 0;	
}
