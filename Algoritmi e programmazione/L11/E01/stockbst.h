#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"
#include "dailystock.h"

#ifndef STOCKBST_H
#define STOCKBST_H

typedef struct sbst *StockBST;

StockBST SBSTinit();
void SBSTfree(StockBST sBst);
DailyStock SBSTsearch(StockBST sBst, Date date);
void SBSTinsertLeafRecursive(StockBST sBst, DailyStock dStock);
DailyStock SBSTmin(StockBST sBst);
DailyStock SBSTmax(StockBST sBst);
void SBSTbalance(StockBST sBst, int S);
void SBSTvisit(StockBST sBst, Date date1, Date date2, float *min, float *max);

#endif
