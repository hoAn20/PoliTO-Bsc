#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"
#include "dailystock.h"
#include "stockbst.h"
#include "stock.h"
#include "stocklist.h"

void MinAndMaxValueInDateInterval(Stock stock);
void MinAndMaxOfAllTime(Stock stock);

int main() {
    FILE *fin = fopen("F3.txt", "r");
    StockLIST sList = SLISTinit();
    int nStock, action, S;
    Stock stock;
    char name[MAX_STR_LEN];

    if (fin == NULL) {
        printf("Errore nell' apertura del file.\n\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d", &nStock);
    for (int i = 0; i < nStock; i++) {
        stock = STOCKinit(fin);
        if (STOCKcompare(SLISTsearch(sList, stock), STOCKsetNull()) == 0) {
            SLISTinsertInOrder(sList, stock);
        }

        for (int j = 0; j < STOCKgetNTransaction(stock); j++)
            SLISTdailyStockUpdate(sList, stock, fin);
    }

    do {
        printf("\n\n 1. ricerca di un titolo azionario\n"
               " 2. ricerca della quotazione massima e minima in un intervallo di date\n"
               " 3. ricerca della quotazione massima e minima in assoluto\n"
               " 4. bilancire il BST di un titolo\n"
               " 0. chiudi\n\n"
               "Inserisci comando: ");
        scanf("%d", &action);
        switch(action) {
            case 0:
                printf("Fine.\n");
                break;
            case 1:
                printf("Inserire il nome del titolo cercato: ");
                scanf("%s", name);
                stock = STOCKfakeName(name);
                stock = SLISTsearch(sList, stock);
                if (STOCKcompare(stock, STOCKsetNull()) == 0)
                    printf("Titolo non presente.\n");
                else
                    STOCKprint(stock);
                break;
            case 2:
                printf("Inserire il nome del titolo cercato: ");
                scanf("%s", name);
                stock = STOCKfakeName(name);
                stock = SLISTsearch(sList, stock);
                MinAndMaxValueInDateInterval(stock);
                break;
            case 3:
                printf("Inserire il nome del titolo cercato: ");
                scanf("%s", name);
                stock = STOCKfakeName(name);
                stock = SLISTsearch(sList, stock);
                MinAndMaxOfAllTime(stock);
                break;
            case 4:
                printf("Inserire il nome del titolo cercato: ");
                scanf("%s", name);
                stock = STOCKfakeName(name);
                printf("Inserisci il valore di soglia: ");
                scanf("%d", &S);
                SBSTbalance(STOCKgetBst(SLISTsearch(sList, stock)), S);
                break;
            default:
                printf("Inserire un comando valido.\n");
        }
    } while (action);

    return 0;
}

void MinAndMaxValueInDateInterval(Stock stock) {
    Date date1, date2;
    char d1[MAX_DATE_LEN], d2[MAX_DATE_LEN];
    float min, max;
    printf("Inserisci la prima data dell intervallo(aaaa/mm/gg): ");
    scanf("%s", d1);
    printf("Inserisci la seconda data dell intervallo(aaaa/mm/gg): ");
    scanf("%s", d2);
    date1 = DATEfromString(d1);
    date2 = DATEfromString(d2);
    SBSTvisit(STOCKgetBst(stock), date1, date2, &min, &max);
    printf("La valutazione minima nell' intervallo di date e': %.2f\n", min);
    printf("Quella massima: %f\n", max);
}
void MinAndMaxOfAllTime(Stock stock) {
    float min, max;
    SBSTvisit(STOCKgetBst(stock), (SBSTmin(STOCKgetBst(stock))).date, (SBSTmax(STOCKgetBst(stock))).date, &min, &max);
    printf("La valutazione minima nell' intervallo di date e': %.2f\n", min);
    printf("Quella massima: %.2f\n", max);
}
