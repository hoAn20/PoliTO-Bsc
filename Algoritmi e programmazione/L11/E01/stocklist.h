#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"
#include "dailystock.h"
#include "stockbst.h"
#include "stock.h"

#ifndef STOCKLIST_H
#define STOCKLIST_H

typedef struct slist *StockLIST;

StockLIST SLISTinit();
void SLISTfree(StockLIST sList);
void SLISTinsertInOrder(StockLIST sList, Stock stock);
Stock SLISTsearch(StockLIST sList, Stock stock);
void SLISTprint(StockLIST sList);
void SLISTdailyStockUpdate(StockLIST sList, Stock stock, FILE *f);

#endif
