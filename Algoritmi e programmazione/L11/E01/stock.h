#include "datetime.h"
#include "dailystock.h"
#include "stockbst.h"

#ifndef STOCK_H
#define STOCK_H

typedef struct stock *Stock;

Stock STOCKinit(FILE *f);
Stock STOCKnew();
void STOCKfree(Stock stock);
int STOCKcompare(Stock stock1, Stock stock2);
char *STOCKgetKey(Stock stock);
StockBST STOCKgetBst(Stock stock);
int STOCKgetNTransaction(Stock stock);
void STOCKaddTransaction(Stock stock, FILE *f);
Stock STOCKsetNull();
void STOCKprint(Stock stock);
Stock STOCKfakeName(char *name);

#endif
