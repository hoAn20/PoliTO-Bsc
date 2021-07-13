#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[]) {
	FILE *f;
	char parola[36], ricerca[36], papabile[36];
	int i, si, count = 0;

	strcpy (ricerca, argv[2]);

	if (argc != 3) {
		printf ("Errore nei parametri inseriti.\n");
		exit (1);
	}
	if ((f = fopen (argv[1], "r")) == NULL) {
		printf ("Impossibile aprire il file specificato.\n");
		exit (1);
	}

	while (!feof (f)) {
		si = 1;
		fscanf (f, "%s", parola);
		if (strlen (ricerca) == strlen (parola)) {
			for (i = 0; i < strlen (ricerca); i++) {
				if (ricerca[i] != '_') {
					if (parola[i] != ricerca[i]) {
						si = 0;
						break;
					}
				}
			}
			if (si) {
				count ++;
				if (count == 1) {
					strcpy (papabile, parola);
				} else if (strcmp (parola, papabile) < 0) {
					strcpy (papabile, parola);
				}
			}
		}
	}
	if (count) {
		printf ("%s - %d combinazioni.\n", papabile, count);
	} else {
		printf ("Nessun suggerimento.\n");
	}
	fclose (f);
	return 0;
}
