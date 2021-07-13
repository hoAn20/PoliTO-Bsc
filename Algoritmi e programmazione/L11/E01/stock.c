#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stock.h"

struct stock {
    char *name;
    StockBST sBst;
    int nTransaction;
    int nStock;
};

Stock STOCKinit(FILE *f) {
    Stock stock = malloc(sizeof(*stock));
    stock->name = malloc((MAX_STR_LEN + 1) * sizeof(char));
    stock->sBst = SBSTinit();
    fscanf(f, "%s %d", stock->name, &stock->nTransaction);

    return stock;
}
Stock STOCKnew() {
    Stock stock;
    stock = malloc(sizeof(*stock));
    stock->name = malloc(MAX_STR_LEN * sizeof(char));
    stock->sBst = SBSTinit();

    return stock;
}
void STOCKfree(Stock stock) {
    free(stock->name);
    SBSTfree(stock->sBst);
    free(stock);
}
int STOCKcompare(Stock stock1, Stock stock2) {
    return strcmp(stock1->name, stock2->name);
}
char *STOCKgetKey(Stock stock) {
    return stock->name;
}
StockBST STOCKgetBst(Stock stock) {
    return stock->sBst;
}
int STOCKgetNTransaction(Stock stock) {
    return stock->nTransaction;
}
void STOCKaddTransaction(Stock stock, FILE *f) {
    DailyStock dStock = DSTOCKnew(f);
    SBSTinsertLeafRecursive(stock->sBst, dStock);
    stock->nStock++;
}
Stock STOCKsetNull() {
    Stock stock = malloc(sizeof(*stock));
    stock->name = malloc(MAX_STR_LEN*sizeof(char));
    strcpy(stock->name, " ");
    stock->nStock = -1;

    return stock;
}
void STOCKprint(Stock stock) {
    printf(" - %s:\t %d %d\n", stock->name, stock->nStock, stock->nTransaction);
}
Stock STOCKfakeName(char *name) {
    Stock stock = malloc(sizeof(*stock));
    stock->name = malloc(MAX_STR_LEN * sizeof(char));
    strcpy(stock->name, name);

    return stock;
}
