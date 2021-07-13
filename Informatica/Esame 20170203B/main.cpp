#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
	FILE *f;
	char max1[13], max2[13], mid1[13], mid2[13];
	float rmax1, rmax2, rimax1, rimax2;
	char user[13];
	float r, ri;
	int dtime, mtime, nclick, npost, nshare, nlike;

	if (argc != 2) {
		printf ("Errore negli argomenti da cmd.\n");
		exit (1);
	}
	if ((f = fopen (argv[1], "r")) == NULL) {
		printf ("Impossibile aprire il file specificato.\n");
		exit (1);
	}
	fscanf (f, "%s %d %d %d %d %d %d", max1, &dtime, &mtime, &nclick, &npost, &nshare, &nlike);
	rmax1 = (float)dtime + 1.5 * mtime + 2 * nclick + 1.8 * npost + 1.5 * nshare + nlike;
	rimax1 = (float)(dtime + mtime) / (nclick + npost + nshare + nlike);
	strcpy (mid1, max1);
	fscanf (f, "%s %d %d %d %d %d %d", user, &dtime, &mtime, &nclick, &npost, &nshare, &nlike);
	r = (float)dtime + 1.5 * mtime + 2 * nclick + 1.8 * npost + 1.5 * nshare + nlike;
	ri = (float)(dtime + mtime) / (nclick + npost + nshare + nlike);
	if (r > rmax1) {
		strcpy (max2, max1);
		strcpy (max1, user);
		rmax2 = rmax1;
		rmax1 = r;
	} else {
		strcpy (max2, user);
		rmax2 = r;
	}
	if (ri > rimax1) {
		strcpy (mid2, mid1);
		strcpy (mid1, user);
		rimax2 = rimax1;
		rimax1 = ri;
	} else {
		strcpy (mid2, user);
		rimax2 = ri;
	}
	while (!feof (f)) {
		fscanf (f, "%s %d %d %d %d %d %d", user, &dtime, &mtime, &nclick, &npost, &nshare, &nlike);
		r = (float)dtime + 1.5 * mtime + 2 * nclick + 1.8 * npost + 1.5 * nshare + nlike;
		ri = (float)(dtime + mtime) / (nclick + npost + nshare + nlike);
		if (r > rmax1) {
			strcpy (max2, max1);
		strcpy (max1, user);
		rmax2 = rmax1;
		rmax1 = r;
		} else if (r > rmax2){
			strcpy (max2, user);
			rmax2 = r;
		}
		if (ri > rimax1) {
			strcpy (mid2, mid1);
			strcpy (mid1, user);
			rimax2 = rimax1;
			rimax1 = ri;
		} else if (ri > rimax2){
			strcpy (mid2, user);
			rimax2 = ri;
		}
	}
	fclose (f);
	printf ("I 2 migliori utenti degli ultimi 30 gg sono:\n\n");
	printf ("%s %.0f\n", max1, rmax1);
	printf ("%s %.0f\n\n", max2, rmax2);
	printf ("I 2 che promettono maggiormente sono:\n\n");
	printf ("%s %.2f\n", mid1, rimax1);
	printf ("%s %.2f\n", mid2, rimax2);
}
