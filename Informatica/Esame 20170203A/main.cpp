#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int main (int argc, char *argv[])
{
	int max = atoi (argv[2]);
	int taskD, errorT;
	int startT, endT, time, startH, startM, startS, endH, endM, endS;
	char bestS[9], worstS[9], serverC[9];
	double taskOT, maxTOT = 0;
	double percE, maxPE = 0;
	int maxE = 0;
	int notF = 1;
	FILE *f;

	if (argc != 3)
	{
		printf ("Errore con i parametri inseriti.\n");
		exit (1);
	}
	if ((f = fopen (argv[1], "r")) == NULL)
	{
		printf ("Impossibile aprire il file specificato\n");
		exit (1);
	}
	if (max <= 0)
	{
		printf ("Numero massimo di applicazioni per server non accettabile.\n");
		exit (1);
	}

	while (!feof (f))
	{
		fscanf (f, "%s %d %d %d:%d:%d %d:%d:%d", serverC, &taskD, &errorT, &startH, &startM, &startS, &endH, &endM, &endS);
		startT = 3600 * startH + 60 * startM + startS;
		endT = 3600 * endH + 60 * endM + endS;
		time = endT - startT;
		taskOT = 3660 * taskD / time;
		if (taskOT > maxTOT)
		{
			maxTOT = taskOT;
			strcpy (bestS, serverC);
		}
		percE = 100 * errorT / taskD;
		if (percE > maxPE)
		{
			maxPE = percE;
			strcpy (worstS, serverC);
			maxE = taskD;
		}
	}
	fclose (f);
	printf ("Maggiori applicazioni corrette:\n%s\n", bestS);
	printf ("Server difettoso:\n%s\n", worstS);
	printf ("Server alternativo:\n");
	f = fopen (argv[1],"r");
	while (!feof (f) && notF) 
	{
		fscanf (f, "%s %d %d %d:%d:%d %d:%d:%d", serverC, &taskD, &errorT, &startH, &startM, &startS, &endH, &endM, &endS);
		if ((taskD + maxE) <= max)
		{
			printf ("%s\n",serverC);
			notF = 0;
		}
	}
	if (notF)
	{
		printf ("Non ho trovato nessun server alternativo\n");
	}
	fclose (f);
	return 0;
}
